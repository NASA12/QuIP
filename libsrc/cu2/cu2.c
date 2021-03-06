
/* jbm's interface to cuda devices */

#include "quip_config.h"

#ifdef HAVE_STRING_H
#include <string.h>
#endif // HAVE_STRING_H

#ifdef HAVE_CUDA

#define BUILD_FOR_CUDA

#include "fileck.h"
#include "my_cu2.h"
#include "cuda_supp.h"

#ifdef HAVE_GL_GLEW_H
#include <GL/glew.h>	// BUG?  do we know we have this file?
#endif // HAVE_GL_GLEW_H

#include "veclib/cu2_veclib_prot.h"
#include "veclib/platform_funcs.h"
#include "../cuda/my_cuda.h"	// query_cuda_device()
#include "cuda_supp.h"
#include <cuda_gl_interop.h>
#include "gl_info.h"

#ifdef HAVE_CUDA
#define DEFAULT_CUDA_ENV_VAR   "DEFAULT_CUDA_DEVICE"
#define DEFAULT_CUDA_DEV_VAR   "default_cuda_device"
static const char *first_cuda_dev_name=NULL;
static const char *default_cuda_dev_name=NULL;
static int default_cuda_dev_found=0;
#endif // HAVE_CUDA

#include "veclib_api.h"
#include "my_cu2.h"	// 
#include "quip_prot.h"	// needs dim3...
#include "cu2_func_tbl.h"


/* On the host 1L<<33 gets us bit 33 - but 1<<33 does not,
 * because, by default, ints are 32 bits.  We don't know
 * how nvcc treats 1L...  but we can try it...
 */

// make these C so we can link from other C files...


#ifdef HAVE_CUDA
// BUG move to platform support file!
static const char * available_pfdev_name(QSP_ARG_DECL  const char *name,char *scratch_string, Compute_Platform *cpp, int max_devices)
{
	Platform_Device *pdp;
	const char *s;
	int n=1;

	s=name;
	while(n<=max_devices){
		pdp = pfdev_of(QSP_ARG  s);
		if( pdp == NO_PFDEV ) return(s);

		// This name is in use
		n++;
		sprintf(scratch_string,"%s_%d",name,n);
		s=scratch_string;
	}
	sprintf(ERROR_STRING,"Number of %s %s devices exceed configured maximum %d!?",
		name,PLATFORM_NAME(cpp),max_devices);
	WARN(ERROR_STRING);
	ERROR1(ERROR_STRING);
	return(NULL);	// NOTREACHED - quiet compiler
}

static void init_cu2_device(QSP_ARG_DECL  int index, Compute_Platform *cpp)
{
	struct cudaDeviceProp deviceProp;
	cudaError_t e;
	Platform_Device *pdp;
	char name[LLEN];
	char dev_name[LLEN];
	char area_name[LLEN];
	const char *name_p;
	char *s;
	Data_Area *ap;
	float comp_cap;

	if( index >= MAX_CUDA_DEVICES ){
		sprintf(ERROR_STRING,"Program is compiled for a maximum of %d CUDA devices, can't inititialize device %d.",
			MAX_CUDA_DEVICES,index);
		ERROR1(ERROR_STRING);
	}

	if( verbose ){
		sprintf(ERROR_STRING,"init_cu2_device %d BEGIN",index);
		advise(ERROR_STRING);
	}

	if( (e=cudaGetDeviceProperties(&deviceProp, index)) != cudaSuccess ){
		describe_cuda_driver_error2("init_cu2_device","cudaGetDeviceProperties",e);
		return;
	}

	if (deviceProp.major == 9999 && deviceProp.minor == 9999){
		sprintf(ERROR_STRING,"There is no CUDA device with dev = %d!?.\n",index);
		WARN(ERROR_STRING);

		/* What should we do here??? */
		return;
	}

	/* Put the compute capability into a script variable so that we can use it */
	comp_cap = deviceProp.major * 10 + deviceProp.minor;
	if( comp_cap > CUDA_COMP_CAP ){
		sprintf(ERROR_STRING,"init_cu2_device:  CUDA device %s has compute capability %d.%d, but program was configured for %d.%d!?",
			deviceProp.name,deviceProp.major,deviceProp.minor,
			CUDA_COMP_CAP/10,CUDA_COMP_CAP%10);
		WARN(ERROR_STRING);
	}

	/* BUG if there are multiple devices, we need to make sure that this is set
	 * correctly for the current context!?
	 */
	sprintf(ERROR_STRING,"%d.%d",deviceProp.major,deviceProp.minor);
	assign_var(QSP_ARG  "cuda_comp_cap",ERROR_STRING);


	/* What does this do??? */
	e = cudaSetDeviceFlags( cudaDeviceMapHost );
	if( e != cudaSuccess ){
		describe_cuda_driver_error2("init_cu2_device",
			"cudaSetDeviceFlags",e);
	}

	strcpy(name,deviceProp.name);

	/* change spaces to underscores */
	s=name;
	while(*s){
		if( *s==' ' ) *s='_';
		s++;
	}

	/* We might have two of the same devices installed in a single system.
	 * In this case, we can't use the device name twice, because there will
	 * be a conflict.  The first one gets the name, then we have to check and
	 * make sure that the name is not in use already.  If it is, then we append
	 * a number to the string...
	 */
	name_p = available_pfdev_name(QSP_ARG  name,dev_name,cpp,MAX_CUDA_DEVICES);	// reuse name as scratch string
	pdp = new_pfdev(QSP_ARG  name_p);

#ifdef CAUTIOUS
	if( pdp == NO_PFDEV ){
		sprintf(ERROR_STRING,"CAUTIOUS:  init_cu2_device:  Error creating cuda device struct for %s!?",name_p);
		WARN(ERROR_STRING);
		return;
	}
#endif /* CAUTIOUS */

	/* Remember this name in case the default is not found */
	if( first_cuda_dev_name == NULL )
		first_cuda_dev_name = PFDEV_NAME(pdp);

	/* Compare this name against the default name set in
	 * the environment, if it exists...
	 */
	if( default_cuda_dev_name != NULL && ! default_cuda_dev_found ){
		if( !strcmp(PFDEV_NAME(pdp),default_cuda_dev_name) )
			default_cuda_dev_found=1;
	}

	SET_PFDEV_PLATFORM(pdp,cpp);
	SET_PFDEV_CUDA_INFO( pdp, getbuf(sizeof(Cuda_Dev_Info)) );

	SET_PFDEV_CUDA_DEV_INDEX(pdp,index);
	SET_PFDEV_CUDA_DEV_PROP(pdp,deviceProp);
	SET_PFDEV_CUDA_RNGEN(pdp,NULL);

	if( comp_cap >= 20 ){
		SET_PFDEV_MAX_DIMS(pdp,3);
	} else {
		SET_PFDEV_MAX_DIMS(pdp,2);
	}

	//set_cuda_device(pdp);	// is this call just so we can call cudaMalloc?
	PF_FUNC_NAME(set_device)(QSP_ARG  pdp);	// is this call just so we can call cudaMalloc?

	// address set to NULL says use custom allocator - see dobj/makedobj.c

	// BUG??  with pdp we may not need the DA_ flag???
	sprintf(area_name,"%s.%s",PLATFORM_NAME(cpp),name_p);
	ap = pf_area_init(QSP_ARG  area_name,NULL,0,
			MAX_CUDA_GLOBAL_OBJECTS,DA_CUDA_GLOBAL,pdp);
	if( ap == NO_AREA ){
		sprintf(ERROR_STRING,
	"init_cu2_device:  error creating global data area %s",area_name);
		WARN(ERROR_STRING);
	}
	// g++ won't take this line!?
	SET_AREA_CUDA_DEV(ap,pdp);
	//set_device_for_area(ap,pdp);

	SET_PFDEV_AREA(pdp,PFDEV_GLOBAL_AREA_INDEX,ap);

	/* We used to declare a heap for constant memory here,
	 * but there wasn't much of a point because:
	 * Constant memory can't be allocated, rather it is declared
	 * in the .cu code, and placed by the compiler as it sees fit.
	 * To have objects use this, we would have to declare a heap and
	 * manage it ourselves...
	 * There's only 64k, so we should be sparing...
	 * We'll try this later...
	 */


	/* Make up another area for the host memory
	 * which is locked and mappable to the device.
	 * We don't allocate a pool here, but do it as needed...
	 */

	//strcpy(area_name,name_p);
	//strcat(area_name,"_host");
	sprintf(area_name,"%s.%s_host",PLATFORM_NAME(cpp),name_p);
	ap = pf_area_init(QSP_ARG  area_name,(u_char *)NULL,0,MAX_CUDA_MAPPED_OBJECTS,
								DA_CUDA_HOST,pdp);
	if( ap == NO_AREA ){
		sprintf(ERROR_STRING,
	"init_cu2_device:  error creating host data area %s",area_name);
		ERROR1(ERROR_STRING);
	}
	SET_AREA_CUDA_DEV(ap, pdp);
	//cuda_data_area[index][CUDA_HOST_AREA_INDEX] = ap;
	SET_PFDEV_AREA(pdp,PFDEV_HOST_AREA_INDEX,ap);

	/* Make up another psuedo-area for the mapped host memory;
	 * This is the same memory as above, but mapped to the device.
	 * In the current implementation, we create objects in the host
	 * area, and then automatically create an alias on the device side.
	 * There is a BUG in that by having this psuedo area in the data
	 * area name space, a user could select it as the data area and
	 * then try to create an object.  We will detect this in make_dobj,
	 * and complain.
	 */

	//strcpy(area_name,name_p);
	//strcat(area_name,"_host_mapped");
	sprintf(area_name,"%s.%s_host_mapped",PLATFORM_NAME(cpp),name_p);
	ap = pf_area_init(QSP_ARG  area_name,(u_char *)NULL,0,MAX_CUDA_MAPPED_OBJECTS,
							DA_CUDA_HOST_MAPPED,pdp);
	if( ap == NO_AREA ){
		sprintf(ERROR_STRING,
	"init_cu2_device:  error creating host-mapped data area %s",area_name);
		ERROR1(ERROR_STRING);
	}
	SET_AREA_CUDA_DEV(ap,pdp);
	//cuda_data_area[index][CUDA_HOST_MAPPED_AREA_INDEX] = ap;
	SET_PFDEV_AREA(pdp,PFDEV_HOST_MAPPED_AREA_INDEX,ap);

	// We don't change the data area by default any more when initializing...
	/* Restore the normal area */
	//set_data_area(PFDEV_AREA(pdp,PFDEV_GLOBAL_AREA_INDEX));

	if( verbose ){
		sprintf(ERROR_STRING,"init_cu2_device %d DONE",index);
		advise(ERROR_STRING);
	}
}

#ifdef NOT_USED
// not used?
int cu2_dispatch( QSP_ARG_DECL  Vector_Function *vfp, Vec_Obj_Args *oap )
{
	int i;

	i = vfp->vf_code;
	if( cu2_func_tbl[i].cu2_func == NULL){
		sprintf(DEFAULT_ERROR_STRING,"Sorry, function %s has not yet been implemented for the cuda2 platform.",VF_NAME(vfp));
		NWARN(DEFAULT_ERROR_STRING);
		return(-1);
	}
// BUG?  why not typtbl???
//fprintf(stderr,"cu2_dispatch calling tabled function %d at 0x%lx\n",
//i,(u_long)vec_func_tbl[i].cu2_func);
//fprintf(stderr,"cu2_dispatch calling function %s\n",
//VF_NAME(&vec_func_tbl[i]));
	(*cu2_func_tbl[i].cu2_func)(VF_CODE(vfp),oap);
	return(0);
}
#endif // NOT_USED
// BUG - these are for host global - can we determine from the data area exactly
// which type of memory we should allocate?

static int cu2_mem_alloc(QSP_ARG_DECL  Data_Obj *dp, dimension_t size, int align)
{
	cudaError_t e;

	// BUG?  align arg is ignored here?

	// GLOBAL
	e = cudaMalloc( &OBJ_DATA_PTR(dp), size);
	if( e != cudaSuccess ){
		if( e == cudaErrorDevicesUnavailable )
			ERROR1("Cuda devices unavailable!?");
		describe_cuda_driver_error2("cu2_mem_alloc","cudaMalloc",e);
		sprintf(ERROR_STRING,"Attempting to allocate %d bytes.",size);
		advise(ERROR_STRING);
		return(-1);
	}
	return 0;
}

static void cu2_mem_free(QSP_ARG_DECL  Data_Obj *dp)
{
	cudaError_t e;

	// GLOBAL
	e = cudaFree(OBJ_DATA_PTR(dp));
	if( e != cudaSuccess ){
		describe_cuda_driver_error2("release_data","cudaFree",e);
	}
}

static void (*cu2_offset_data)(QSP_ARG_DECL  Data_Obj *dp, index_t o ) = default_offset_data_func;
static void cu2_update_offset(QSP_ARG_DECL  Data_Obj *dp)
{ WARN("cu2_update_offset not implemented!?"); }

static void cu2_mem_dnload(QSP_ARG_DECL  void *dst, void *src, size_t siz, Platform_Device *pdp )
{
#ifdef HAVE_CUDA

	cudaError_t error;

        //cutilSafeCall( cutilDeviceSynchronize() );	// added for 4.0?
#ifdef OLD_CUDA4
	cutilSafeCall( cudaMemcpy(dst, src, siz, cudaMemcpyDeviceToHost) );
#else // ! OLD_CUDA4
fprintf(stderr,"cu2_mem_dnload:  dst = 0x%lx   src = 0x%lx   siz = %ld\n",
(long)dst,(long)src,siz);
	error = cudaMemcpy(dst, src, siz, cudaMemcpyDeviceToHost) ;
	if( error != cudaSuccess ){
		// BUG report cuda error
		WARN("Error in cudaMemcpy, device to host!?");
		sprintf(ERROR_STRING,"dst = 0x%lx, src = 0x%lx, size = %ld",
			(u_long) dst, (u_long) src, siz );
		advise(ERROR_STRING);
		describe_cuda_driver_error2("cu2_mem_dnload",
				"cudaMemcpy",error);
	}
#endif // ! OLD_CUDA4
#else // ! HAVE_CUDA
	NO_CUDA_MSG(mem_dnload)
#endif // ! HAVE_CUDA
}

// We treat the device as a server, so "upload" transfers from host to device

static void cu2_mem_upload(QSP_ARG_DECL  void *dst, void *src, size_t siz, Platform_Device *pdp )
{
#ifdef HAVE_CUDA
	cudaError_t error;

#ifdef OLD_CUDA4
	cutilSafeCall( cudaMemcpy(dst, src, siz, cudaMemcpyHostToDevice) );
#else // ! OLD_CUDA4

	// BUG?  do we need to make sure that the correct device is selected?

	error = cudaMemcpy(dst, src, siz, cudaMemcpyHostToDevice);
	if( error != cudaSuccess ){
		// BUG report cuda error
		WARN("Error in cudaMemcpy, host to device!?");
	}
#endif // ! OLD_CUDA4
#else // ! HAVE_CUDA
	NO_CUDA_MSG(mem_upload)
#endif // ! HAVE_CUDA
}

// For Cuda, we create an object and then

static int cu2_register_buf(QSP_ARG_DECL  Data_Obj *dp)
{
#ifdef HAVE_OPENGL
	cudaError_t e;

	/* how do we check for an error? */
	e = cudaGLRegisterBufferObject( OBJ_BUF_ID(dp) );
	if( e != cudaSuccess ){
		describe_cuda_driver_error2("cu2_register_buf",
				"cudaGLRegisterBufferObject",e);
		return -1;
	}
	return 0;
#else // ! HAVE_OPENGL
	WARN("cu2_register_buf:  Sorry, no OpenGL support in this build!?");
	return -1;
#endif // ! HAVE_OPENGL
}

static int cu2_map_buf(QSP_ARG_DECL  Data_Obj *dp)
{
#ifdef HAVE_OPENGL
	cudaError_t e;

	e = cudaGLMapBufferObject( &OBJ_DATA_PTR(dp),  OBJ_BUF_ID(dp) );
	if( e != cudaSuccess ){
		describe_cuda_driver_error2("cu2_map_buf",
				"cudaGLMapBufferObject",e);
		return -1;
	}
	return 0;
#else // ! HAVE_OPENGL
	WARN("cu2_map_buf:  Sorry, no OpenGL support in this build!?");
	return -1;
#endif // ! HAVE_OPENGL
}

static int cu2_unmap_buf(QSP_ARG_DECL  Data_Obj *dp)
{
#ifdef HAVE_OPENGL
	cudaError_t e;

	e = cudaGLUnmapBufferObject( OBJ_BUF_ID(dp) );   
	if( e != cudaSuccess ){
		describe_cuda_driver_error2("cu2_unmap_buf",
			"cudaGLUnmapBufferObject",e);
		ERROR1("failed to unmap buffer object");
		return -1;
	}
	return 0;
#else // ! HAVE_OPENGL
	WARN("cu2_unmap_buf:  Sorry, no OpenGL support in this build!?");
	return -1;
#endif // ! HAVE_OPENGL
}

static void cu2_dev_info(QSP_ARG_DECL  Platform_Device *pdp)
{
	sprintf(MSG_STR,"%s:",PFDEV_NAME(pdp));
	prt_msg(MSG_STR);

	// should print info from device query...
	//prt_msg("Sorry, Cuda-specific device info not implemented yet!?");

	query_cuda_device(QSP_ARG  PFDEV_CUDA_DEV_INDEX(pdp) );
}

static void cu2_info(QSP_ARG_DECL  Compute_Platform *cdp)
{
	sprintf(MSG_STR,"%s:",PLATFORM_NAME(cdp));
	prt_msg(MSG_STR);

	// Should print the names of the available devices...
	prt_msg("Sorry, Cuda-specific platform info not implemented yet!?");
}


static int init_cu2_devices(QSP_ARG_DECL  Compute_Platform *cpp)
{
	int n_devs,i;

	/* We don't get a proper error message if the cuda files in /dev
	 * are not readable...  So we check that first.
	 */

	if( check_file_access(QSP_ARG  "/dev/nvidiactl") < 0 )
		return -1;

	cudaGetDeviceCount(&n_devs);

	if( n_devs == 0 ){
		WARN("No CUDA devices found!?");
		return -1;
	}

	if( verbose ){
		sprintf(ERROR_STRING,"%d cuda devices found...",n_devs);
		advise(ERROR_STRING);
	}

	default_cuda_dev_name = getenv(DEFAULT_CUDA_ENV_VAR);
	/* may be null */

	for(i=0;i<n_devs;i++){
		char s[32];

		sprintf(s,"/dev/nvidia%d",i);
		if( check_file_access(QSP_ARG  s) < 0 ){
			// BUG do we need to unwind some things
			// at this point?
			//
			// Probably not a big concern, because if the
			// device files are missing then nvidia_ctl
			// is also likely to be missing, trapped above...
			return -1;
		}

		init_cu2_device(QSP_ARG  i, cpp);
	}

	if( default_cuda_dev_name == NULL ){
		/* Not specified in environment */
		// reserved var if set in environment?
		assign_var(QSP_ARG  DEFAULT_CUDA_DEV_VAR,first_cuda_dev_name);
		default_cuda_dev_found=1;	// not really necessary?
	} else if( ! default_cuda_dev_found ){
		/* specified incorrectly */
		sprintf(ERROR_STRING, "%s %s not found.\nUsing device %s.",
			DEFAULT_CUDA_DEV_VAR,default_cuda_dev_name,
			first_cuda_dev_name);
		WARN(ERROR_STRING);

		assign_var(QSP_ARG  DEFAULT_CUDA_DEV_VAR,first_cuda_dev_name);
		default_cuda_dev_found=1;	// not really necessary?
	}

	/* hopefully the vector library is already initialized - can we be sure? */

	SET_PLATFORM_FUNCTIONS(cpp,cu2)
	SET_PF_FUNC_TBL(cpp,cu2_vfa_tbl);
	//SET_PLATFORM_DISPATCH_TBL(cpp,cu2_func_tbl);

	return 0;
} // end init_cu2_devices
#endif // HAVE_CUDA

void cu2_init_platform(SINGLE_QSP_ARG_DECL)
{
	static int inited=0;

	if( inited ){
		//WARN("Redundant call to cu2_init_platform!?");
		advise("Redundant call to cu2_init_platform!?");
		return;
	}
	inited=1;
#ifdef HAVE_CUDA
	Compute_Platform *cpp;

	cpp = creat_platform(QSP_ARG  "CUDA", PLATFORM_CUDA );

	//init_platform(QSP_ARG  cpp, PLATFORM_CUDA);

#ifdef CAUTIOUS
	if( cpp == NULL )
ERROR1("CAUTIOUS:  cu2_init_platform:  Couldn't create Cuda2 platform!?");
#endif // CAUTIOUS

	push_pfdev_context(QSP_ARG  PF_CONTEXT(cpp) );
	if( init_cu2_devices(QSP_ARG  cpp) < 0 ){
		/*	/dev/nvidia_ctl may be missing after a reboot
		 */
		inited = -1;
	}
	if( pop_pfdev_context(SINGLE_QSP_ARG) == NO_ITEM_CONTEXT )
		ERROR1("cu2_init_platform:  Failed to pop platform device context!?");

	check_vfa_tbl(QSP_ARG  cu2_vfa_tbl, N_VEC_FUNCS);

	if( inited < 0 ){
		// problem above
		delete_platform(QSP_ARG  cpp);
	}

#else // ! HAVE_CUDA
	WARN("Sorry, no CUDA support in this build.");
#endif // ! HAVE_CUDA
} // cu2_init_platform



// a utility used in host_calls.h
#define MAXD(m,n)	(m>n?m:n)
#define MAX2(szi_p)	MAXD(szi_p->szi_dst_dim[i_dim],szi_p->szi_src_dim[1][i_dim])
#define MAX3(szi_p)	MAXD(MAX2(szi_p),szi_p->szi_src_dim[1][i_dim])


PF_COMMAND_FUNC( list_devs )
{
	list_pfdevs(SINGLE_QSP_ARG);
}

void PF_FUNC_NAME(set_device)( QSP_ARG_DECL  Platform_Device *pdp )
{
#ifdef HAVE_CUDA
	cudaError_t e;
#endif // HAVE_CUDA

	if( curr_pdp == pdp ){
		sprintf(DEFAULT_ERROR_STRING,"%s:  current device is already %s!?",
			STRINGIFY(HOST_CALL_NAME(set_device)),PFDEV_NAME(pdp));
		NWARN(DEFAULT_ERROR_STRING);
		return;
	}
	if( PFDEV_PLATFORM_TYPE(pdp) != PLATFORM_CUDA ){
		sprintf(ERROR_STRING,"%s:  device %s is not a CUDA device!?",
			STRINGIFY(HOST_CALL_NAME(set_device)),PFDEV_NAME(pdp));
		WARN(ERROR_STRING);
		return;
	}

#ifdef HAVE_CUDA
	e = cudaSetDevice( PFDEV_CUDA_DEV_INDEX(pdp) );
	if( e != cudaSuccess )
		describe_cuda_driver_error2(STRINGIFY(HOST_CALL_NAME(set_device)),"cudaSetDevice",e);
	else
		curr_pdp = pdp;
#else // ! HAVE_CUDA
	NO_CUDA_MSG(set_device)
#endif // ! HAVE_CUDA
}

void insure_cu2_device( QSP_ARG_DECL  Data_Obj *dp )
{
	Platform_Device *pdp;

	if( AREA_FLAGS(OBJ_AREA(dp)) & DA_RAM ){
		sprintf(DEFAULT_ERROR_STRING,
	"insure_cu2_device:  Object %s is a host RAM object!?",OBJ_NAME(dp));
		NWARN(DEFAULT_ERROR_STRING);
		return;
	}

	pdp = AREA_PFDEV(OBJ_AREA(dp));

#ifdef CAUTIOUS
	if( pdp == NULL )
		NERROR1("CAUTIOUS:  null cuda device ptr in data area!?");
#endif /* CAUTIOUS */

	if( curr_pdp != pdp ){
sprintf(DEFAULT_ERROR_STRING,"insure_cu2_device:  curr_pdp = 0x%lx  pdp = 0x%lx",
(int_for_addr)curr_pdp,(int_for_addr)pdp);
NADVISE(DEFAULT_ERROR_STRING);

sprintf(DEFAULT_ERROR_STRING,"insure_cu2_device:  current device is %s, want %s",
PFDEV_NAME(curr_pdp),PFDEV_NAME(pdp));
NADVISE(DEFAULT_ERROR_STRING);
		PF_FUNC_NAME(set_device)(QSP_ARG  pdp);
	}

}

void *TMPVEC_NAME(size_t size,size_t len,const char *whence)
{
/*
	void *cuda_mem;
	cudaError_t drv_err;

	drv_err = cudaMalloc(&cuda_mem, size * len );
	if( drv_err != cudaSuccess ){
		sprintf(DEFAULT_MSG_STR,"tmpvec (%s)",whence);
		describe_cuda_driver_error2(DEFAULT_MSG_STR,"cudaMalloc",drv_err);
		NERROR1("CUDA memory allocation error");
	}

//sprintf(ERROR_STRING,"tmpvec:  %d bytes allocated at 0x%lx",len,(int_for_addr)cuda_mem);
//advise(ERROR_STRING);

//sprintf(ERROR_STRING,"tmpvec %s:  0x%lx",whence,(int_for_addr)cuda_mem);
//advise(ERROR_STRING);
	return(cuda_mem);
	*/
	return NULL;
}

void FREETMP_NAME(void *ptr,const char *whence)
{
	/*
	cudaError_t drv_err;

//sprintf(ERROR_STRING,"freetmp %s:  0x%lx",whence,(int_for_addr)ptr);
//advise(ERROR_STRING);
	drv_err=cudaFree(ptr);
	if( drv_err != cudaSuccess ){
		sprintf(DEFAULT_MSG_STR,"freetmp (%s)",whence);
		describe_cuda_driver_error2(DEFAULT_MSG_STR,"cudaFree",drv_err);
	}
	*/
}

#ifdef FOOBAR
//CUFFT
//static const char* getCUFFTError(cufftResult_t status)

void g_fwdfft(QSP_ARG_DECL  Data_Obj *dst_dp, Data_Obj *src1_dp)
{
	//Variable declarations
	int NX = 256;
	//int BATCH = 10;
	int BATCH = 1;
	cufftResult_t status;

	//Declare plan for FFT
	cufftHandle plan;
	//cufftComplex *data;
	//cufftComplex *result;
	void *data;
	void *result;
	cudaError_t drv_err;

	//Allocate RAM
	//cutilSafeCall(cudaMalloc(&data, sizeof(cufftComplex)*NX*BATCH));	
	//cutilSafeCall(cudaMalloc(&result, sizeof(cufftComplex)*NX*BATCH));
	drv_err = cudaMalloc(&data, sizeof(cufftComplex)*NX*BATCH);
	if( drv_err != cudaSuccess ){
		WARN("error allocating cuda data buffer for fft!?");
		return;
	}
	drv_err = cudaMalloc(&result, sizeof(cufftComplex)*NX*BATCH);
	if( drv_err != cudaSuccess ){
		WARN("error allocating cuda result buffer for fft!?");
		// BUG clean up previous malloc...
		return;
	}

	//Create plan for FFT
	status = cufftPlan1d(&plan, NX, CUFFT_C2C, BATCH);
	if (status != CUFFT_SUCCESS) {
		sprintf(ERROR_STRING, "Error in cufftPlan1d: %s\n", getCUFFTError(status));
		NWARN(ERROR_STRING);
	}

	//Run forward fft on data
	status = cufftExecC2C(plan, (cufftComplex *)data,
			(cufftComplex *)result, CUFFT_FORWARD);
	if (status != CUFFT_SUCCESS) {
		sprintf(ERROR_STRING, "Error in cufftExecC2C: %s\n", getCUFFTError(status));
		NWARN(ERROR_STRING);
	}

	//Run inverse fft on data
	/*status = cufftExecC2C(plan, data, result, CUFFT_INVERSE);
	if (status != CUFFT_SUCCESS)
	{
		sprintf(ERROR_STRING, "Error in cufftExecC2C: %s\n", getCUFFTError(status));
		NWARN(ERROR_STRING);
	}*/

	//Free resources
	cufftDestroy(plan);
	cudaFree(data);
}
#endif // FOOBAR


typedef struct {
	const char *	ckpt_tag;
#ifdef FOOBAR
	cudaEvent_t	ckpt_event;
#endif // FOOBAR
} CU2_Checkpoint;

static CU2_Checkpoint *ckpt_tbl=NULL;
//static int max_cu2_ckpts=0;	// size of checkpoit table
static int n_cu2_ckpts=0;	// number of placements

static void init_cu2_ckpts(int n)
{
	/*
	//CUresult e;
	cudaError_t drv_err;
	int i;

	if( max_cu2_ckpts > 0 ){
		sprintf(DEFAULT_ERROR_STRING,
"init_cu2_ckpts (%d):  already initialized with %d checpoints",
			n,max_cu2_ckpts);
		NWARN(DEFAULT_ERROR_STRING);
		return;
	}
	ckpt_tbl = (Cuda_Checkpoint *) getbuf( n * sizeof(*ckpt_tbl) );
	if( ckpt_tbl == NULL ) NERROR1("failed to allocate checkpoint table");

	max_cu2_ckpts = n;

	for(i=0;i<max_cu2_ckpts;i++){
		drv_err=cudaEventCreate(&ckpt_tbl[i].ckpt_event);
		if( drv_err != cudaSuccess ){
			describe_cuda_driver_error2("init_cu2_ckpts",
				"cudaEventCreate",drv_err);
			NERROR1("failed to initialize checkpoint table");
		}
		ckpt_tbl[i].ckpt_tag=NULL;
	}
	*/
}

PF_COMMAND_FUNC( init_ckpts )
{
	int n;

	n = HOW_MANY("maximum number of checkpoints");
	init_cu2_ckpts(n);
}

#define CUDA_DRIVER_ERROR_RETURN(calling_funcname, cuda_funcname )	\
								\
	if( drv_err != cudaSuccess ){					\
		describe_cuda_driver_error2(calling_funcname,cuda_funcname,drv_err); \
		return;						\
	}


#define CUDA_ERROR_RETURN(calling_funcname, cuda_funcname )	\
								\
	if( e != CUDA_SUCCESS ){					\
		describe_cuda_error2(calling_funcname,cuda_funcname,e); \
		return;						\
	}


#define CUDA_ERROR_FATAL(calling_funcname, cuda_funcname )	\
								\
	if( e != CUDA_SUCCESS ){					\
		describe_cuda_error2(calling_funcname,cuda_funcname,e); \
		ERROR1("Fatal cuda error.");			\
	}


PF_COMMAND_FUNC( set_ckpt )
{
	/*
	//cudaError_t e;
	cudaError_t drv_err;
	const char *s;

	s = NAMEOF("tag for this checkpoint");

	if( max_cu2_ckpts == 0 ){
		NWARN("do_place_ckpt:  checkpoint table not initialized, setting to default size");
		init_cu2_ckpts(256);
	}

	if( n_cu2_ckpts >= max_cu2_ckpts ){
		sprintf(ERROR_STRING,
	"do_place_ckpt:  Sorry, all %d checkpoints have already been placed",
			max_cu2_ckpts);
		WARN(ERROR_STRING);
		return;
	}

	ckpt_tbl[n_cu2_ckpts].ckpt_tag = savestr(s);

	// use default stream (0) for now, but will want to introduce
	// more streams later?
	drv_err = cudaEventRecord( ckpt_tbl[n_cu2_ckpts++].ckpt_event, 0 );
	CUDA_DRIVER_ERROR_RETURN( "do_place_ckpt","cudaEventRecord")
	*/
}

PF_COMMAND_FUNC( show_ckpts )
{
	/*
	CUresult e;
	cudaError_t drv_err;
	float msec, cum_msec;
	int i;

	if( n_cu2_ckpts <= 0 ){
		NWARN("do_show_cu2_ckpts:  no checkpoints placed!?");
		return;
	}

	drv_err = cudaEventSynchronize(ckpt_tbl[n_cu2_ckpts-1].ckpt_event);
	CUDA_DRIVER_ERROR_RETURN("do_show_cu2_ckpts", "cudaEventSynchronize")

	drv_err = cudaEventElapsedTime( &msec, ckpt_tbl[0].ckpt_event, ckpt_tbl[n_cu2_ckpts-1].ckpt_event);
	CUDA_DRIVER_ERROR_RETURN("do_show_cu2_ckpts", "cudaEventElapsedTime")
	sprintf(msg_str,"Total GPU time:\t%g msec",msec);
	prt_msg(msg_str);

	// show the start tag
	sprintf(msg_str,"GPU  %3d  %12.3f  %12.3f  %s",1,0.0,0.0,
		ckpt_tbl[0].ckpt_tag);
	prt_msg(msg_str);
	cum_msec =0.0;
	for(i=1;i<n_cu2_ckpts;i++){
		drv_err = cudaEventElapsedTime( &msec, ckpt_tbl[i-1].ckpt_event,
			ckpt_tbl[i].ckpt_event);
		CUDA_DRIVER_ERROR_RETURN("do_show_cu2_ckpts", "cudaEventElapsedTime")

		cum_msec += msec;
		sprintf(msg_str,"GPU  %3d  %12.3f  %12.3f  %s",i+1,msec,
			cum_msec, ckpt_tbl[i].ckpt_tag);
		prt_msg(msg_str);
	}
	*/
}

PF_COMMAND_FUNC( clear_ckpts )
{
	int i;

	for(i=0;i<n_cu2_ckpts;i++){
		rls_str(ckpt_tbl[i].ckpt_tag);
		ckpt_tbl[i].ckpt_tag=NULL;
	}
	n_cu2_ckpts=0;
}




#endif /* HAVE_CUDA */

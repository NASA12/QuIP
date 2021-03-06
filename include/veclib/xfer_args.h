
// The NOCC functions have a destination array of indices, and a source vector of inputs,
// plus two return scalars

// Can't use XFER_FAST_ARGS_2, because it uses dst_dp for the count...
// But we need to know the length of the index array (dest)

#define XFER_FAST_ARGS_NOCC		XFER_SLOW_ARGS_1		\
					XFER_FAST_COUNT(SRC1_DP)	\
					XFER_FAST_ARGS_SRC1		\
					XFER_RETSCAL1 XFER_RETSCAL2

#define XFER_EQSP_ARGS_NOCC		SET_VA_FLAGS(vap,VA_EQSP_ARGS);		\
					XFER_SLOW_ARGS_1			\
					XFER_FAST_COUNT(SRC1_DP)		\
					XFER_EQSP_ARGS_SRC1			\
					XFER_RETSCAL1 XFER_RETSCAL2

#define XFER_SLOW_ARGS_NOCC		XFER_SLOW_ARGS_2	\
					XFER_RETSCAL1		\
					XFER_RETSCAL2

#define XFER_FAST_ARGS_SBM_3		XFER_FAST_ARGS_SBM XFER_FAST_ARGS_3
#define XFER_EQSP_ARGS_SBM_3		XFER_EQSP_ARGS_SBM XFER_EQSP_ARGS_3
#define XFER_SLOW_ARGS_SBM_3		XFER_SLOW_ARGS_SBM XFER_SLOW_ARGS_3

#define XFER_FAST_ARGS_SBM_1		XFER_FAST_ARGS_SBM XFER_FAST_ARGS_1
#define XFER_EQSP_ARGS_SBM_1		XFER_EQSP_ARGS_SBM XFER_EQSP_ARGS_1
#define XFER_SLOW_ARGS_SBM_1		XFER_SLOW_ARGS_SBM XFER_SLOW_ARGS_1

#define XFER_FAST_ARGS_SBM_1S_2		XFER_FAST_ARGS_SBM XFER_FAST_ARGS_1S_2
#define XFER_EQSP_ARGS_SBM_1S_2		XFER_EQSP_ARGS_SBM XFER_EQSP_ARGS_1S_2
#define XFER_SLOW_ARGS_SBM_1S_2		XFER_SLOW_ARGS_SBM XFER_SLOW_ARGS_1S_2

#define XFER_FAST_ARGS_SBM_2S_1		XFER_FAST_ARGS_SBM XFER_FAST_ARGS_2S_1
#define XFER_EQSP_ARGS_SBM_2S_1		XFER_EQSP_ARGS_SBM XFER_EQSP_ARGS_2S_1
#define XFER_SLOW_ARGS_SBM_2S_1		XFER_SLOW_ARGS_SBM XFER_SLOW_ARGS_2S_1

#define XFER_FAST_ARGS_SBM_CPX_3	XFER_FAST_ARGS_SBM XFER_FAST_ARGS_CPX_3
#define XFER_EQSP_ARGS_SBM_CPX_3	XFER_EQSP_ARGS_SBM XFER_EQSP_ARGS_CPX_3
#define XFER_SLOW_ARGS_SBM_CPX_3	XFER_SLOW_ARGS_SBM XFER_SLOW_ARGS_CPX_3

#define XFER_FAST_ARGS_SBM_CPX_1S_2	XFER_FAST_ARGS_SBM XFER_FAST_ARGS_CPX_1S_2
#define XFER_EQSP_ARGS_SBM_CPX_1S_2	XFER_EQSP_ARGS_SBM XFER_EQSP_ARGS_CPX_1S_2
#define XFER_SLOW_ARGS_SBM_CPX_1S_2	XFER_SLOW_ARGS_SBM XFER_SLOW_ARGS_CPX_1S_2

#define XFER_FAST_ARGS_SBM_CPX_2S_1	XFER_FAST_ARGS_SBM XFER_FAST_ARGS_CPX_2S_1
#define XFER_EQSP_ARGS_SBM_CPX_2S_1	XFER_EQSP_ARGS_SBM XFER_EQSP_ARGS_CPX_2S_1
#define XFER_SLOW_ARGS_SBM_CPX_2S_1	XFER_SLOW_ARGS_SBM XFER_SLOW_ARGS_CPX_2S_1

#define XFER_FAST_ARGS_SBM_QUAT_3	XFER_FAST_ARGS_SBM XFER_FAST_ARGS_QUAT_3
#define XFER_EQSP_ARGS_SBM_QUAT_3	XFER_EQSP_ARGS_SBM XFER_EQSP_ARGS_QUAT_3
#define XFER_SLOW_ARGS_SBM_QUAT_3	XFER_SLOW_ARGS_SBM XFER_SLOW_ARGS_QUAT_3

#define XFER_FAST_ARGS_SBM_QUAT_1S_2	XFER_FAST_ARGS_SBM XFER_FAST_ARGS_QUAT_1S_2
#define XFER_EQSP_ARGS_SBM_QUAT_1S_2	XFER_EQSP_ARGS_SBM XFER_EQSP_ARGS_QUAT_1S_2
#define XFER_SLOW_ARGS_SBM_QUAT_1S_2	XFER_SLOW_ARGS_SBM XFER_SLOW_ARGS_QUAT_1S_2

#define XFER_FAST_ARGS_SBM_QUAT_2S_1	XFER_FAST_ARGS_SBM XFER_FAST_ARGS_QUAT_2S_1
#define XFER_EQSP_ARGS_SBM_QUAT_2S_1	XFER_EQSP_ARGS_SBM XFER_EQSP_ARGS_QUAT_2S_1
#define XFER_SLOW_ARGS_SBM_QUAT_2S_1	XFER_SLOW_ARGS_SBM XFER_SLOW_ARGS_QUAT_2S_1

#define XFER_FAST_ARGS_DBM_2SRCS	XFER_FAST_ARGS_DBM_1SRC XFER_FAST_ARGS_SRC2
#define XFER_EQSP_ARGS_DBM_2SRCS	XFER_EQSP_ARGS_DBM_1SRC XFER_FAST_ARGS_SRC2
#define XFER_SLOW_ARGS_DBM_2SRCS	XFER_SLOW_ARGS_DBM XFER_SLOW_ARGS_2SRCS

#define XFER_FAST_ARGS_DBM_1S_		XFER_FAST_ARGS_DBM XFER_ARGS_1S
#define XFER_EQSP_ARGS_DBM_1S_		XFER_EQSP_ARGS_DBM XFER_ARGS_1S
#define XFER_SLOW_ARGS_DBM_1S_		XFER_SLOW_ARGS_DBM XFER_ARGS_1S

#define XFER_FAST_ARGS_DBM_1S_1SRC	XFER_FAST_ARGS_DBM_1SRC XFER_ARGS_1S
#define XFER_EQSP_ARGS_DBM_1S_1SRC	XFER_EQSP_ARGS_DBM_1SRC XFER_ARGS_1S
#define XFER_SLOW_ARGS_DBM_1S_1SRC	XFER_SLOW_ARGS_DBM_1SRC XFER_ARGS_1S

#define XFER_FAST_ARGS_DBM_1SRC	XFER_FAST_ARGS_DBM XFER_FAST_ARGS_SRC1 XFER_FAST_COUNT(SRC1_DP)
#define XFER_EQSP_ARGS_DBM_1SRC	XFER_EQSP_ARGS_DBM XFER_EQSP_ARGS_SRC1 XFER_EQSP_COUNT(SRC1_DP)
#define XFER_SLOW_ARGS_DBM_1SRC	XFER_SLOW_ARGS_DBM XFER_SLOW_ARGS_SRC1

// for rvmov, sbm is passed as src1?
#define XFER_FAST_ARGS_DBM_SBM	XFER_FAST_ARGS_DBM XFER_FAST_ARGS_SBM XFER_FAST_COUNT(bitmap_src_dp)
#define XFER_EQSP_ARGS_DBM_SBM	XFER_EQSP_ARGS_DBM XFER_FAST_ARGS_SBM XFER_EQSP_COUNT(bitmap_src_dp)
#define XFER_SLOW_ARGS_DBM_SBM	XFER_SLOW_ARGS_DBM XFER_SLOW_ARGS_SBM


#define XFER_FAST_ARGS_DBM_1S	XFER_FAST_ARGS_DBM XFER_ARGS_1S XFER_FAST_COUNT(bitmap_dst_dp)
#define XFER_EQSP_ARGS_DBM_1S	XFER_EQSP_ARGS_DBM XFER_ARGS_1S XFER_EQSP_COUNT(bitmap_dst_dp)
#define XFER_SLOW_ARGS_DBM_1S	XFER_SLOW_ARGS_DBM XFER_ARGS_1S

#define XFER_FAST_ARGS_3S_1	XFER_FAST_ARGS_1 XFER_ARGS_3S
#define XFER_EQSP_ARGS_3S_1	XFER_EQSP_ARGS_1 XFER_ARGS_3S
#define XFER_SLOW_ARGS_3S_1	XFER_SLOW_ARGS_1 XFER_ARGS_3S

#define XFER_FAST_ARGS_3S_2	XFER_FAST_ARGS_2 XFER_ARGS_3S
#define XFER_EQSP_ARGS_3S_2	XFER_EQSP_ARGS_2 XFER_ARGS_3S
#define XFER_SLOW_ARGS_3S_2	XFER_SLOW_ARGS_2 XFER_ARGS_3S

#define XFER_FAST_ARGS_2S_3	XFER_FAST_ARGS_3 XFER_ARGS_2S
#define XFER_EQSP_ARGS_2S_3	XFER_EQSP_ARGS_3 XFER_ARGS_2S
#define XFER_SLOW_ARGS_2S_3	XFER_SLOW_ARGS_3 XFER_ARGS_2S

#define XFER_FAST_ARGS_1S_4	XFER_FAST_ARGS_4 XFER_ARGS_1S
#define XFER_EQSP_ARGS_1S_4	XFER_EQSP_ARGS_4 XFER_ARGS_1S
#define XFER_SLOW_ARGS_1S_4	XFER_SLOW_ARGS_4 XFER_ARGS_1S

#define XFER_FAST_ARGS_1S_1	XFER_FAST_ARGS_1 XFER_ARGS_1S
#define XFER_EQSP_ARGS_1S_1	XFER_EQSP_ARGS_1 XFER_ARGS_1S
#define XFER_SLOW_ARGS_1S_1	XFER_SLOW_ARGS_1 XFER_ARGS_1S

#define XFER_FAST_ARGS_1S_2	XFER_FAST_ARGS_2 XFER_ARGS_1S
#define XFER_EQSP_ARGS_1S_2	XFER_EQSP_ARGS_2 XFER_ARGS_1S
#define XFER_SLOW_ARGS_1S_2	XFER_SLOW_ARGS_2 XFER_ARGS_1S

#define XFER_FAST_ARGS_2S_1	XFER_FAST_ARGS_1 XFER_ARGS_2S
#define XFER_EQSP_ARGS_2S_1	XFER_EQSP_ARGS_1 XFER_ARGS_2S
#define XFER_SLOW_ARGS_2S_1	XFER_SLOW_ARGS_1 XFER_ARGS_2S

#define XFER_FAST_ARGS_2SRCS	XFER_FAST_ARGS_SRC1 XFER_FAST_ARGS_SRC2
#define XFER_EQSP_ARGS_2SRCS	XFER_EQSP_ARGS_SRC1 XFER_EQSP_ARGS_SRC2
#define XFER_SLOW_ARGS_2SRCS	XFER_SLOW_ARGS_SRC1 XFER_SLOW_ARGS_SRC2

#define XFER_FAST_ARGS_CPX_1	XFER_FAST_ARGS_1
#define XFER_EQSP_ARGS_CPX_1	XFER_EQSP_ARGS_1
#define XFER_SLOW_ARGS_CPX_1	XFER_SLOW_ARGS_1

#define XFER_FAST_ARGS_CPX_2	XFER_FAST_ARGS_2
#define XFER_EQSP_ARGS_CPX_2	XFER_EQSP_ARGS_2
#define XFER_SLOW_ARGS_CPX_2	XFER_SLOW_ARGS_2

#define XFER_FAST_ARGS_CPX_3	XFER_FAST_ARGS_3
#define XFER_EQSP_ARGS_CPX_3	XFER_EQSP_ARGS_3
#define XFER_SLOW_ARGS_CPX_3	XFER_SLOW_ARGS_3

#define XFER_FAST_ARGS_CPX_2S_1	XFER_FAST_ARGS_1 XFER_ARGS_CPX_2S
#define XFER_EQSP_ARGS_CPX_2S_1	XFER_EQSP_ARGS_1 XFER_ARGS_CPX_2S
#define XFER_SLOW_ARGS_CPX_2S_1	XFER_SLOW_ARGS_1 XFER_ARGS_CPX_2S

#define XFER_FAST_ARGS_QUAT_1	XFER_FAST_ARGS_1
#define XFER_EQSP_ARGS_QUAT_1	XFER_EQSP_ARGS_1
#define XFER_SLOW_ARGS_QUAT_1	XFER_SLOW_ARGS_1

#define XFER_FAST_ARGS_QUAT_2	XFER_FAST_ARGS_2
#define XFER_EQSP_ARGS_QUAT_2	XFER_EQSP_ARGS_2
#define XFER_SLOW_ARGS_QUAT_2	XFER_SLOW_ARGS_2

#define XFER_FAST_ARGS_QUAT_3	XFER_FAST_ARGS_3
#define XFER_EQSP_ARGS_QUAT_3	XFER_EQSP_ARGS_3
#define XFER_SLOW_ARGS_QUAT_3	XFER_SLOW_ARGS_3

#define XFER_FAST_ARGS_QUAT_2S_1	XFER_FAST_ARGS_1 XFER_ARGS_QUAT_2S
#define XFER_EQSP_ARGS_QUAT_2S_1	XFER_EQSP_ARGS_1 XFER_ARGS_QUAT_2S
#define XFER_SLOW_ARGS_QUAT_2S_1	XFER_SLOW_ARGS_1 XFER_ARGS_QUAT_2S

#define XFER_FAST_ARGS_RC_2	XFER_FAST_ARGS_2
#define XFER_EQSP_ARGS_RC_2	XFER_EQSP_ARGS_2
#define XFER_SLOW_ARGS_RC_2	XFER_SLOW_ARGS_2

#define XFER_FAST_ARGS_CR_2	XFER_FAST_ARGS_2
#define XFER_EQSP_ARGS_CR_2	XFER_EQSP_ARGS_2
#define XFER_SLOW_ARGS_CR_2	XFER_SLOW_ARGS_2

#define XFER_FAST_ARGS_CCR_3	XFER_FAST_ARGS_3
#define XFER_EQSP_ARGS_CCR_3	XFER_EQSP_ARGS_3
#define XFER_SLOW_ARGS_CCR_3	XFER_SLOW_ARGS_3

#define XFER_FAST_ARGS_CR_1S_2	XFER_FAST_ARGS_1S_2
#define XFER_EQSP_ARGS_CR_1S_2	XFER_EQSP_ARGS_1S_2
#define XFER_SLOW_ARGS_CR_1S_2	XFER_SLOW_ARGS_1S_2

#define XFER_FAST_ARGS_CPX_1S_2	XFER_FAST_ARGS_2 XFER_ARGS_CPX_1S
#define XFER_EQSP_ARGS_CPX_1S_2	XFER_EQSP_ARGS_2 XFER_ARGS_CPX_1S
#define XFER_SLOW_ARGS_CPX_1S_2	XFER_SLOW_ARGS_2 XFER_ARGS_CPX_1S

#define XFER_FAST_ARGS_CPX_1S_1	XFER_FAST_ARGS_1 XFER_ARGS_CPX_1S
#define XFER_EQSP_ARGS_CPX_1S_1	XFER_EQSP_ARGS_1 XFER_ARGS_CPX_1S
#define XFER_SLOW_ARGS_CPX_1S_1	XFER_SLOW_ARGS_1 XFER_ARGS_CPX_1S

#define XFER_FAST_ARGS_RQ_2	XFER_FAST_ARGS_2
#define XFER_EQSP_ARGS_RQ_2	XFER_EQSP_ARGS_2
#define XFER_SLOW_ARGS_RQ_2	XFER_SLOW_ARGS_2

#define XFER_FAST_ARGS_QR_2	XFER_FAST_ARGS_2
#define XFER_EQSP_ARGS_QR_2	XFER_EQSP_ARGS_2
#define XFER_SLOW_ARGS_QR_2	XFER_SLOW_ARGS_2

#define XFER_FAST_ARGS_QQR_3	XFER_FAST_ARGS_3
#define XFER_EQSP_ARGS_QQR_3	XFER_EQSP_ARGS_3
#define XFER_SLOW_ARGS_QQR_3	XFER_SLOW_ARGS_3

#define XFER_FAST_ARGS_QR_1S_2	XFER_FAST_ARGS_1S_2
#define XFER_EQSP_ARGS_QR_1S_2	XFER_EQSP_ARGS_1S_2
#define XFER_SLOW_ARGS_QR_1S_2	XFER_SLOW_ARGS_1S_2

#define XFER_FAST_ARGS_QUAT_1S_2	XFER_FAST_ARGS_2 XFER_ARGS_QUAT_1S
#define XFER_EQSP_ARGS_QUAT_1S_2	XFER_EQSP_ARGS_2 XFER_ARGS_QUAT_1S
#define XFER_SLOW_ARGS_QUAT_1S_2	XFER_SLOW_ARGS_2 XFER_ARGS_QUAT_1S

#define XFER_FAST_ARGS_QUAT_1S_1	XFER_FAST_ARGS_1 XFER_ARGS_QUAT_1S
#define XFER_EQSP_ARGS_QUAT_1S_1	XFER_EQSP_ARGS_1 XFER_ARGS_QUAT_1S
#define XFER_SLOW_ARGS_QUAT_1S_1	XFER_SLOW_ARGS_1 XFER_ARGS_QUAT_1S

#define XFER_FAST_ARGS_CONV	XFER_FAST_ARGS_2

#define XFER_FAST_ARGS_2	XFER_FAST_ARGS_1 XFER_FAST_ARGS_SRC1

/*#define XFER_ARGS_1S						\
 *	SET_VA_SVAL1(vap, (oap->oa_svp[0])->std_scalar );
 */

#define XFER_ARGS_1S						\
	SET_VA_SVAL1(vap, OA_SVAL(oap,0) );

#define XFER_ARGS_CPX_1S					\
	SET_VA_CVAL1(vap, OA_CPX_SVAL(oap,0) );

#define XFER_ARGS_CPX_2S					\
	XFER_ARGS_CPX_1S					\
	SET_VA_CVAL2(vap, OA_CPX_SVAL(oap,1) );

#define XFER_ARGS_QUAT_1S					\
	SET_VA_QVAL1(vap,OA_QUAT_SVAL(oap,0) );

#define XFER_ARGS_QUAT_2S					\
	XFER_ARGS_QUAT_1S					\
	SET_VA_QVAL2(vap, OA_QUAT_SVAL(oap,1) );

#define XFER_ARGS_3S						\
	XFER_ARGS_2S						\
	SET_VA_SVAL3(vap, OA_SVAL(oap,2) );

#define XFER_ARGS_2S						\
	XFER_ARGS_1S						\
	SET_VA_SVAL2(vap, OA_SVAL(oap,1) );


#define XFER_FAST_ARGS_DBM								\
				XFER_DBM_PTR						\
				XFER_FAST_COUNT(bitmap_dst_dp)				\
				SET_VA_DBM_BIT0(vap, OBJ_BIT0(bitmap_dst_dp) );

#define XFER_FAST_ARGS_SBM								\
				XFER_SBM_PTR						\
				SET_VA_SBM_BIT0(vap, OBJ_BIT0(bitmap_src_dp) );

#define XFER_EQSP_ARGS_DBM								\
				SET_VA_FLAGS(vap,VA_EQSP_ARGS);				\
				XFER_FAST_ARGS_DBM					\
				XFER_EQSP_DBM_GPU_INFO /* nop on CPU */			\
				SET_VA_DBM_BIT0(vap, OBJ_BIT0(bitmap_dst_dp) );		\
				XFER_EQSP_INC_DBM

#define XFER_EQSP_ARGS_SBM								\
				XFER_SBM_PTR						\
				SET_VA_SBM_BIT0(vap, OBJ_BIT0(bitmap_src_dp) );

#define XFER_SLOW_ARGS_DBM	SLOW_ARGS_SETUP						\
				XFER_DBM_PTR						\
				XFER_SLOW_DBM_GPU_INFO					\
				SET_VA_DEST_INCSET(vap, OBJ_TYPE_INCS(bitmap_dst_dp) );	\
				SET_VA_COUNT(vap, OBJ_TYPE_DIMS(bitmap_dst_dp) );	\
				SET_VA_DBM_BIT0(vap, OBJ_BIT0(bitmap_dst_dp) );

#define XFER_SLOW_ARGS_SBM	SLOW_ARGS_SETUP						\
				XFER_SBM_PTR						\
				SET_VA_SBM_INCSET(vap, OBJ_TYPE_INCS(bitmap_src_dp) );	\
				SET_VA_SBM_DIMSET(vap, OBJ_TYPE_DIMS(bitmap_src_dp) );	\
				SET_VA_SBM_BIT0(vap, OBJ_BIT0(bitmap_src_dp) );



#define XFER_FAST_ARGS_2	XFER_FAST_ARGS_1 XFER_FAST_ARGS_SRC1
#define XFER_SLOW_ARGS_CONV	XFER_SLOW_ARGS_2
#define XFER_SLOW_ARGS_2	XFER_SLOW_ARGS_1 XFER_SLOW_ARGS_SRC1
#define XFER_FAST_ARGS_3	XFER_FAST_ARGS_2 XFER_FAST_ARGS_SRC2
#define XFER_SLOW_ARGS_3	XFER_SLOW_ARGS_2 XFER_SLOW_ARGS_SRC2
#define XFER_FAST_ARGS_4	XFER_FAST_ARGS_3 XFER_FAST_ARGS_SRC3
#define XFER_SLOW_ARGS_4	XFER_SLOW_ARGS_3 XFER_SLOW_ARGS_SRC3
#define XFER_FAST_ARGS_5	XFER_FAST_ARGS_4 XFER_FAST_ARGS_SRC4
#define XFER_SLOW_ARGS_5	XFER_SLOW_ARGS_4 XFER_SLOW_ARGS_SRC4

#define SHOW_ARG_DP(stem)	sprintf(error_string,"%s object:  %s",#stem,stem##_dp->dt_name);\
				advise(error_string);

#define XFER_DEST_PTR		SET_VA_DEST_PTR(vap, OBJ_DATA_PTR(dst_dp) );	\
				SET_VA_DEST_OFFSET(vap,OBJ_OFFSET(dst_dp));

#define SRC_DP(idx)		OA_SRC_OBJ(oap,idx)

/*#define XFER_SRC_PTR(idx)	SET_VA_SRC_PTR(vap,idx, OBJ_DATA_PTR(SRC_DP(idx)) );*/
#define XFER_SRC_PTR(idx)	SET_VA_SRC_PTR(vap,idx, OBJ_DATA_PTR(SRC_DP(idx)) );	\
				SET_VA_SRC_OFFSET(vap,idx,OBJ_OFFSET(SRC_DP(idx)) );


#define XFER_SRC1_PTR		XFER_SRC_PTR(0)
#define XFER_SRC2_PTR		XFER_SRC_PTR(1)
#define XFER_SRC3_PTR		XFER_SRC_PTR(2)
#define XFER_SRC4_PTR		XFER_SRC_PTR(3)
#define XFER_SRC5_PTR		XFER_SRC_PTR(4)

#define XFER_RETSCAL1		SET_VA_SVAL(vap,0,OBJ_DATA_PTR(OA_SCLR_OBJ(oap,0)));
#define XFER_RETSCAL2		SET_VA_SVAL(vap,1,OBJ_DATA_PTR(OA_SCLR_OBJ(oap,1)));

#define XFER_DBM_PTR		SET_VA_DEST_PTR(vap, OBJ_DATA_PTR(bitmap_dst_dp) );
#define XFER_SBM_PTR		SET_VA_SBM_PTR(vap, OBJ_DATA_PTR(bitmap_src_dp) );

// XFER_DBM_GPU_INFO needs to be defined differently for cpu & gpu !!!

#ifdef MOVED
#define XFER_DBM_GPU_INFO	if( BITMAP_OBJ_GPU_INFO_HOST_PTR(bitmap_dst_dp) == NULL ){				\
					/* only for gpu objects! */							\
					init_bitmap_gpu_info(bitmap_dst_dp);						\
				}											\
				SET_VA_DBM_GPU_INFO_PTR(vap, BITMAP_OBJ_GPU_INFO_DEV_PTR(bitmap_dst_dp));
#endif // MOVED

#define XFER_FAST_ARGS_1	XFER_DEST_PTR					\
				XFER_FAST_COUNT(dst_dp)

#define XFER_FAST_COUNT(dp)	SET_VA_LEN(vap, OBJ_N_TYPE_ELTS(dp) );
#define XFER_EQSP_COUNT(dp)	SET_VA_LEN(vap, OBJ_N_TYPE_ELTS(dp) );	// same as XFER_FAST_COUNT !?

#define XFER_FAST_ARGS_SRC(idx)	XFER_SRC_PTR(idx)
#define XFER_FAST_ARGS_SRC1	XFER_FAST_ARGS_SRC(0)
#define XFER_FAST_ARGS_SRC2	XFER_FAST_ARGS_SRC(1)
#define XFER_FAST_ARGS_SRC3	XFER_FAST_ARGS_SRC(2)
#define XFER_FAST_ARGS_SRC4	XFER_FAST_ARGS_SRC(3)

#define XFER_SRC1_PTR		XFER_SRC_PTR(0)

#define SLOW_ARGS_SETUP		/* no-op, mem is allocated at object creation... */
	

#define XFER_SLOW_ARGS_1					\
	SLOW_ARGS_SETUP						\
	XFER_DEST_PTR						\
	SET_VA_DEST_INCSET(vap, OBJ_TYPE_INCS(dst_dp) );	\
	SET_VA_COUNT(vap,OBJ_TYPE_DIMS(dst_dp) );		\
/*show_vec_args(vap);*/


// Why don't we transfer the destination dimset???

#define XFER_SLOW_ARGS_SRC(idx)	XFER_SRC_PTR(idx)			\
				XFER_SLOW_SRC_SHAPE(idx)		\
/*show_vec_args(vap);*/

#define XFER_SLOW_ARGS_SRC1	XFER_SLOW_ARGS_SRC(0)
#define XFER_SLOW_ARGS_SRC2	XFER_SLOW_ARGS_SRC(1)
#define XFER_SLOW_ARGS_SRC3	XFER_SLOW_ARGS_SRC(2)
#define XFER_SLOW_ARGS_SRC4	XFER_SLOW_ARGS_SRC(3)

#define XFER_SLOW_SRC_SHAPE(idx)				\
								\
	SET_VA_SRC_INCSET(vap,idx,OBJ_TYPE_INCS(SRC_DP(idx)) );	\
	SET_VA_SRC_DIMSET(vap,idx,OBJ_TYPE_DIMS(SRC_DP(idx)) );


#define XFER_EQSP_ARGS_CONV	XFER_EQSP_ARGS_2
#define XFER_EQSP_ARGS_2	XFER_EQSP_ARGS_1 XFER_EQSP_ARGS_SRC1
#define XFER_EQSP_ARGS_3	XFER_EQSP_ARGS_2 XFER_EQSP_ARGS_SRC2
#define XFER_EQSP_ARGS_4	XFER_EQSP_ARGS_3 XFER_EQSP_ARGS_SRC3
#define XFER_EQSP_ARGS_5	XFER_EQSP_ARGS_4 XFER_EQSP_ARGS_SRC4

//#define XFER_DST_INCR(dp)	fprintf(stderr,"XFER_DST_INCR\n");  longlist(DEFAULT_QSP_ARG  dp); SET_VA_DEST_INCSET(vap,OBJ_TYPE_INCS(dp) );
#define XFER_DST_INCR(dp)	SET_VA_DEST_INCSET(vap,OBJ_TYPE_INCS(dp) );
#define XFER_SRC1_INCR(dp)	SET_VA_SRC1_INCSET(vap,OBJ_TYPE_INCS(dp) );
#define XFER_SRC2_INCR(dp)	SET_VA_SRC2_INCSET(vap,OBJ_TYPE_INCS(dp) );
#define XFER_SRC3_INCR(dp)	SET_VA_SRC3_INCSET(vap,OBJ_TYPE_INCS(dp) );
#define XFER_SRC4_INCR(dp)	SET_VA_SRC4_INCSET(vap,OBJ_TYPE_INCS(dp) );

#define XFER_EQSP_DEST_INC(dp)	SET_VA_DEST_EQSP_INC(vap, OBJ_TYPE_INC(dp,OBJ_MINDIM(dp) ) );

//#define XFER_EQSP_INC_1		SET_VA_DEST_EQSP_INC(vap, OBJ_TYPE_INC(OA_DEST(oap),OBJ_MINDIM(OA_DEST(oap) ) ) );
//#define XFER_EQSP_INC_DBM	SET_VA_DEST_EQSP_INC(vap, OBJ_TYPE_INC(bitmap_dst_dp,OBJ_MINDIM(bitmap_dst_dp ) ) );
#define XFER_EQSP_INC_1		XFER_EQSP_DEST_INC( OA_DEST(oap) )
#define XFER_EQSP_INC_DBM	XFER_EQSP_DEST_INC( bitmap_dst_dp )

// XFER_FAST_ARGS_1 transfers the data ptr and the total count

#define XFER_EQSP_ARGS_1	SET_VA_FLAGS(vap,VA_EQSP_ARGS);		\
				XFER_FAST_ARGS_1			\
				XFER_EQSP_INC_1


#define XFER_EQSP_ARGS_SRC(idx)	XFER_FAST_ARGS_SRC(idx)			\
				SET_VA_SRC_EQSP_INC(vap,idx,OBJ_TYPE_INC(OA_SRC_OBJ(oap,idx),OBJ_MINDIM(OA_SRC_OBJ(oap,idx) ) ) );

#define XFER_EQSP_ARGS_SRC1	XFER_EQSP_ARGS_SRC(0)
#define XFER_EQSP_ARGS_SRC2	XFER_EQSP_ARGS_SRC(1)
#define XFER_EQSP_ARGS_SRC3	XFER_EQSP_ARGS_SRC(2)
#define XFER_EQSP_ARGS_SRC4	XFER_EQSP_ARGS_SRC(3)


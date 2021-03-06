#include "quip_config.h"

#include <stdio.h>

#ifdef HAVE_STRING_H
#include <string.h>
#endif

#include "debug.h"
#include "viewer.h"
#include "view_cmds.h"
#include "view_util.h"
#include "view_prot.h"
#include "cmaps.h"
#include "xsupp.h"

static int viewer_name_in_use(QSP_ARG_DECL const char *s)
{
	Viewer *vp;

	vp = VWR_OF(s);
	if( vp != NO_VIEWER){
		sprintf(ERROR_STRING,"viewer name \"%s\" in use",s);
		WARN(ERROR_STRING);
		return(1);
	}
	return(0);
}

static Viewer * mk_new_viewer(QSP_ARG_DECL int viewer_type)
{
	const char *s;
	char name[256];
	int dx,dy;
	Viewer *vp;

	s=NAMEOF("viewer name");
	strcpy(name,s);
	dx=(int)HOW_MANY("width");
	dy=(int)HOW_MANY("height");
	if( viewer_name_in_use(QSP_ARG name) ) return NO_VIEWER;
	if( dx <= 0 || dy <= 0 ){
		WARN("viewer sizes must be positive");
		return NO_VIEWER;
	}
	vp = viewer_init(QSP_ARG  name,dx,dy,viewer_type);

	if( vp == NO_VIEWER ) return NO_VIEWER;
#ifdef HAVE_X11
	default_cmap(QSP_ARG  VW_DPYABLE(vp) );
#endif /* HAVE_X11 */
#ifndef BUILD_FOR_IOS
	/* default state is to be shown,
	 * but in IOS we can only see one at a time, so
	 * we leave them on the bottom until we ask.
	 */
	show_viewer(QSP_ARG  vp);
#endif /* ! BUILD_FOR_IOS */
	select_viewer(QSP_ARG  vp);
	return vp;
}

COMMAND_FUNC( mk_viewer )
{
	if( mk_new_viewer(QSP_ARG 0) == NO_VIEWER )
		WARN("Error creating viewer!?");
}

COMMAND_FUNC( mk_pixmap )
{
	if( mk_new_viewer(QSP_ARG VIEW_PIXMAP) == NO_VIEWER )
		WARN("Error creating pixmap!?");
}

COMMAND_FUNC( mk_plotter )
{
	Viewer *vp;

	if( (vp=mk_new_viewer(QSP_ARG VIEW_PLOTTER)) == NO_VIEWER )
		WARN("Error creating plotter!?");
#ifdef BUILD_FOR_IOS
	else
		init_viewer_canvas(vp);
#endif /* BUILD_FOR_IOS */
}

COMMAND_FUNC( mk_2d_adjuster )
{
	Viewer *vp;
	const char *s;

	vp=mk_new_viewer(QSP_ARG VIEW_ADJUSTER);
	s=NAMEOF("action text");
	if( vp == NO_VIEWER ) return;
	SET_VW_TEXT(vp, savestr(s) );
}

COMMAND_FUNC( mk_gl_window )
{
	if( mk_new_viewer(QSP_ARG VIEW_GL) == NO_VIEWER )
		WARN("Error creating gl_window!?");
}

//#define MAX_ACTION_LEN		512	// BUG check for overrun or use string buffer...

COMMAND_FUNC( mk_button_arena )
{
	Viewer *vp;
	//char b1[MAX_ACTION_LEN],b2[MAX_ACTION_LEN],b3[MAX_ACTION_LEN];
	const char *b1,*b2,*b3;

	vp=mk_new_viewer(QSP_ARG VIEW_BUTTON_ARENA);
	// Do we really have to copy these?
	b1=savestr(NAMEOF("left button text"));
	b2=savestr(NAMEOF("middle button text"));
	b3=savestr(NAMEOF("right button text"));
	if( vp == NO_VIEWER ){
		rls_str(b1);
		rls_str(b2);
		rls_str(b3);
	} else {
		SET_VW_TEXT1(vp, b1 );
		SET_VW_TEXT2(vp, b2 );
		SET_VW_TEXT3(vp, b3 );
	}
}

// reset_button_funcs was written when the only events
// came from a 3 button mouse.  Under IOS we have touch
// events instead.  We leave this for the time being for
// backwards compatibility, but replace it with a more
// general scheme...

COMMAND_FUNC( reset_button_funcs )
{
	Viewer *vp;
	//char b1[LLEN],b2[LLEN],b3[LLEN];
	const char *b1, *b2, *b3;

	vp = PICK_VWR("");

	b1=savestr(NAMEOF("left button text"));
	b2=savestr(NAMEOF("middle button text"));
	b3=savestr(NAMEOF("right button text"));

advise("NOTE:  reset_button_funcs:  actions command is deprecated, use event_action instead");
	if( vp == NO_VIEWER ){
		rls_str(b1);
		rls_str(b2);
		rls_str(b3);
	} else {
		// BUG?  do we know these are set already?
		rls_str((char *)VW_TEXT1(vp));
		rls_str((char *)VW_TEXT2(vp));
		rls_str((char *)VW_TEXT3(vp));

		SET_VW_TEXT1(vp, b1 );
		SET_VW_TEXT2(vp, b2 );
		SET_VW_TEXT3(vp, b3 );
	}
}

COMMAND_FUNC( do_set_event_action )
{
	Viewer *vp;
	Canvas_Event *cep;
	const char *action_text;

	vp = PICK_VWR("");
	cep = PICK_CANVAS_EVENT("event type");
	action_text = NAMEOF("action text");

	if( vp == NO_VIEWER ) return;
	if( cep == NO_CANVAS_EVENT ) return;
    
	set_action_for_event(vp,cep,action_text);
}

COMMAND_FUNC( mk_mousescape )
{
	Viewer *vp;
	const char *s;

	vp=mk_new_viewer(QSP_ARG VIEW_MOUSESCAPE);
	s=NAMEOF("action text");
	if( vp == NO_VIEWER ) return;
	SET_VW_TEXT(vp, savestr(s));
}

COMMAND_FUNC( reset_window_text )
{
	const char *s;
	Viewer *vp;

	vp=PICK_VWR("");
	s=NAMEOF("window action text");

	if( vp == NO_VIEWER) return;
	if( VW_TEXT(vp) != NULL ) rls_str((char *)VW_TEXT(vp));

	SET_VW_TEXT(vp, savestr(s) );
}

COMMAND_FUNC( mk_dragscape )
{
	if( mk_new_viewer(QSP_ARG VIEW_DRAGSCAPE) == NO_VIEWER )
		WARN("Error creating dragscape!?");
}

COMMAND_FUNC( do_redraw )
{
	Viewer *vp;

	vp=PICK_VWR("");
	if( vp == NO_VIEWER) return;

	INSURE_X11_SERVER
	redraw_viewer(QSP_ARG  vp);
	select_viewer(QSP_ARG  vp);
}

COMMAND_FUNC( do_embed_image )
{
	Viewer *vp;
	Data_Obj *dp;
	int x,y;

	vp = PICK_VWR("");
	dp = PICK_OBJ("image");
	x=(int)HOW_MANY("x position");
	y=(int)HOW_MANY("y position");

	if( vp == NO_VIEWER || dp == NO_OBJ ){
		WARN("can't embed image");
		return;
	}

	INSIST_RAM_OBJ(dp,"embed_image");

	INSURE_X11_SERVER

#ifdef BUILD_FOR_IOS
	embed_image(QSP_ARG  vp,dp,x,y);
#else /* ! BUILD_FOR_IOS */

	// what does add_image do???
	if( add_image(vp,dp,x,y) ){
		embed_image(QSP_ARG  vp,dp,x,y);
	} else {
		bring_image_to_front(QSP_ARG  vp,dp,x,y);
	}

#endif /* ! BUILD_FOR_IOS */
}

// This reads an image out of the viewer.
// This is used when we can't render into a memory buffer
// (e.g., X11 text drawing)

COMMAND_FUNC( do_unembed_image )
{
	Viewer *vp;
	Data_Obj *dp;
	int x,y;

	dp = PICK_OBJ("image");
	vp = PICK_VWR("");
	x=(int)HOW_MANY("x position");
	y=(int)HOW_MANY("y position");

	if( vp == NO_VIEWER || dp == NO_OBJ ){
		WARN("can't unembed image");
		return;
	}

	INSIST_RAM_OBJ(dp,"unembed_image");

	INSURE_X11_SERVER
	unembed_image(QSP_ARG  vp,dp,x,y);
}

COMMAND_FUNC( do_load_viewer )
{
	Viewer *vp;
	Data_Obj *dp;

	vp = PICK_VWR("");
	dp = PICK_OBJ("image");

	if( vp == NO_VIEWER || dp == NO_OBJ ) return;

	INSIST_RAM_OBJ(dp,"load_viewer");

	INSURE_X11_SERVER
//fprintf(stderr,"Calling load_viewer %s %s\n",VW_NAME(vp),OBJ_NAME(dp));
	load_viewer(QSP_ARG  vp,dp);
	select_viewer(QSP_ARG  vp);
}


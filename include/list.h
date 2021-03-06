#ifndef _LIST_H_
#define _LIST_H_

#include "quip_fwd.h"
#include "node.h"
#include "typedefs.h"
#include "item_obj.h"

#ifdef THREAD_SAFE_QUERY
extern int n_active_threads;	// Number of qsp's
#endif /* THREAD_SAFE_QUERY */

struct list {
	Node *		l_head;
	Node *		l_tail;
#ifdef THREAD_SAFE_QUERY
#ifdef HAVE_PTHREADS
	pthread_mutex_t	l_mutex;
	int		l_flags;	// Flags
#endif /* HAVE_PTHREADS */
#endif /* THREAD_SAFE_QUERY */
} ;

#define INIT_LIST(lp)		{ lp->l_head=NO_NODE; lp->l_tail=NO_NODE; }
#define ALLOC_LIST		((List *)getbuf(sizeof(List)))

#ifdef THREAD_SAFE_QUERY

#define LIST_LOCKED	1

#ifdef HAVE_PTHREADS

#define LIST_IS_LOCKED(lp)	(lp->l_flags&LIST_LOCKED)

#define LOCK_LIST(lp,whence)						\
								\
	if( n_active_threads > 1 )				\
	{							\
		int status;					\
								\
/*fprintf(stderr,"%s:  locking list 0x%lx\n",#whence,(long)lp);*/\
		status = pthread_mutex_lock(&lp->l_mutex);	\
		if( status != 0 )				\
			report_mutex_error(DEFAULT_QSP_ARG  status,"LOCK_LIST");	\
		lp->l_flags |= LIST_LOCKED;			\
/*fprintf(stderr,"%s:  list 0x%lx is locked\n",#whence,(long)lp);*/\
	}

#define UNLOCK_LIST(lp,whence)						\
								\
	if( LIST_IS_LOCKED(lp) )				\
	{							\
		int status;					\
								\
		lp->l_flags &= ~LIST_LOCKED;			\
/*fprintf(stderr,"%s:  unlocking list 0x%lx\n",#whence,(long)lp);*/\
		status = pthread_mutex_unlock(&lp->l_mutex);	\
		if( status != 0 )				\
			report_mutex_error(DEFAULT_QSP_ARG  status,"UNLOCK_LIST");\
/*fprintf(stderr,"%s:  list 0x%lx is unlocked\n\n",#whence,(long)lp);*/\
	}

#else /* ! HAVE_PTHREADS */

#define LOCK_LIST(lp)
#define UNLOCK_LIST(lp)
#define LIST_IS_LOCKED(lp)	0

#endif /* ! HAVE_PTHREADS */

#else /* ! THREAD_SAFE_QUERY */

#define LOCK_LIST(lp)
#define UNLOCK_LIST(lp)

#endif /* ! THREAD_SAFE_QUERY */

#define NO_LIST		((List *)NULL)

/* sys/queue.h defines LIST_HEAD also!? */
#define QLIST_HEAD(lp)	lp->l_head
//#define LIST_HEAD(lp)	lp->l_head
#define QLIST_TAIL(lp)	lp->l_tail
#define SET_QLIST_HEAD(lp,np)	lp->l_head = np
#define SET_QLIST_TAIL(lp,np)	lp->l_tail = np

#define IS_EMPTY(lp)	(QLIST_HEAD(lp)==NO_NODE)

typedef struct {
	List *lp;
	Node *np;
} List_Enumerator;

extern void advance_list_enumerator(List_Enumerator *lep);
extern Item *list_enumerator_item(List_Enumerator *lep);
extern List_Enumerator *new_list_enumerator(List *lp);
extern void rls_list_enumerator(List_Enumerator *lp);

#endif /* ! _LIST_H_ */


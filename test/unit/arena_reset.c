#ifndef ARENA_RESET_PROF_C_
#include "test/jemalloc_test.h"
#endif

#include "jemalloc/internal/extent_mmap.h"
#include "jemalloc/internal/rtree.h"

#include "test/extent_hooks.h"

// static unsigned
// get_nsizes_impl(const char *cmd) {
// 	unsigned ret;
// 	size_t z;

// 	z = sizeof(unsigned);
// 	assert_d_eq(mallctl(cmd, (void *)&ret, &z, NULL, 0), 0,
// 	    "Unexpected mallctl(\"%s\", ...) failure", cmd);

// 	return ret;
// }

// static unsigned
// get_nsmall(void) {
// 	return get_nsizes_impl("arenas.nbins");
// }

// static unsigned
// get_nlarge(void) {
// 	return get_nsizes_impl("arenas.nlextents");
// }

// static size_t
// get_size_impl(const char *cmd, size_t ind) {
// 	size_t ret;
// 	size_t z;
// 	size_t mib[4];
// 	size_t miblen = 4;

// 	z = sizeof(size_t);
// 	assert_d_eq(mallctlnametomib(cmd, mib, &miblen),
// 	    0, "Unexpected mallctlnametomib(\"%s\", ...) failure", cmd);
// 	mib[2] = ind;
// 	z = sizeof(size_t);
// 	assert_d_eq(mallctlbymib(mib, miblen, (void *)&ret, &z, NULL, 0),
// 	    0, "Unexpected mallctlbymib([\"%s\", %zu], ...) failure", cmd, ind);

// 	return ret;
// }

// static size_t
// get_small_size(size_t ind) {
// 	return get_size_impl("arenas.bin.0.size", ind);
// }

// static size_t
// get_large_size(size_t ind) {
// 	return get_size_impl("arenas.lextent.0.size", ind);
// }

// /* Like ivsalloc(), but safe to call on discarded allocations. */
// static size_t
// vsalloc(tsdn_t *tsdn, const void *ptr) {
// 	rtree_ctx_t rtree_ctx_fallback;
// 	rtree_ctx_t *rtree_ctx = tsdn_rtree_ctx(tsdn, &rtree_ctx_fallback);

// 	extent_t *extent;
// 	szind_t szind;
// 	if (rtree_extent_szind_read(tsdn, &extents_rtree, rtree_ctx,
// 	    (uintptr_t)ptr, false, &extent, &szind)) {
// 		return 0;
// 	}

// 	if (extent == NULL) {
// 		return 0;
// 	}
// 	if (extent_state_get(extent) != extent_state_active) {
// 		return 0;
// 	}

// 	if (szind == SC_NSIZES) {
// 		return 0;
// 	}

// 	return sz_index2size(szind);
// }

// static unsigned
// do_arena_create(extent_hooks_t *h) {
// 	unsigned arena_ind;
// 	size_t sz = sizeof(unsigned);
// 	assert_d_eq(mallctl("arenas.create", (void *)&arena_ind, &sz,
// 	    (void *)(h != NULL ? &h : NULL), (h != NULL ? sizeof(h) : 0)), 0,
// 	    "Unexpected mallctl() failure");
// 	return arena_ind;
// }

// static void
// do_arena_reset_pre(unsigned arena_ind, void ***ptrs, unsigned *nptrs) {
// #define NLARGE	32
// 	unsigned nsmall, nlarge, i;
// 	size_t sz;
// 	int flags;
// 	tsdn_t *tsdn;

// 	flags = MALLOCX_ARENA(arena_ind) | MALLOCX_TCACHE_NONE;

// 	nsmall = get_nsmall();
// 	nlarge = get_nlarge() > NLARGE ? NLARGE : get_nlarge();
// 	*nptrs = nsmall + nlarge;
// 	*ptrs = (void **)malloc(*nptrs * sizeof(void *));
// 	assert_ptr_not_null(*ptrs, "Unexpected malloc() failure");

// 	/* Allocate objects with a wide range of sizes. */
// 	for (i = 0; i < nsmall; i++) {
// 		sz = get_small_size(i);
// 		(*ptrs)[i] = mallocx(sz, flags);
// 		assert_ptr_not_null((*ptrs)[i],
// 		    "Unexpected mallocx(%zu, %#x) failure", sz, flags);
// 	}
// 	for (i = 0; i < nlarge; i++) {
// 		sz = get_large_size(i);
// 		(*ptrs)[nsmall + i] = mallocx(sz, flags);
// 		assert_ptr_not_null((*ptrs)[i],
// 		    "Unexpected mallocx(%zu, %#x) failure", sz, flags);
// 	}

// 	tsdn = tsdn_fetch();

// 	/* Verify allocations. */
// 	for (i = 0; i < *nptrs; i++) {
// 		assert_zu_gt(ivsalloc(tsdn, (*ptrs)[i]), 0,
// 		    "Allocation should have queryable size");
// 	}
// }

// static void
// do_arena_reset_post(void **ptrs, unsigned nptrs, unsigned arena_ind) {
// 	tsdn_t *tsdn;
// 	unsigned i;

// 	tsdn = tsdn_fetch();

// 	if (have_background_thread) {
// 		malloc_mutex_lock(tsdn,
// 		    &background_thread_info_get(arena_ind)->mtx);
// 	}
// 	/* Verify allocations no longer exist. */
// 	for (i = 0; i < nptrs; i++) {
// 		assert_zu_eq(vsalloc(tsdn, ptrs[i]), 0,
// 		    "Allocation should no longer exist");
// 	}
// 	if (have_background_thread) {
// 		malloc_mutex_unlock(tsdn,
// 		    &background_thread_info_get(arena_ind)->mtx);
// 	}

// 	free(ptrs);
// }

// static void
// do_arena_reset_destroy(const char *name, unsigned arena_ind) {
// 	size_t mib[3];
// 	size_t miblen;

// 	miblen = sizeof(mib)/sizeof(size_t);
// 	assert_d_eq(mallctlnametomib(name, mib, &miblen), 0,
// 	    "Unexpected mallctlnametomib() failure");
// 	mib[1] = (size_t)arena_ind;
// 	assert_d_eq(mallctlbymib(mib, miblen, NULL, NULL, NULL, 0), 0,
// 	    "Unexpected mallctlbymib() failure");
// }

// static void
// do_arena_reset(unsigned arena_ind) {
// 	do_arena_reset_destroy("arena.0.reset", arena_ind);
// }

// static void
// do_arena_destroy(unsigned arena_ind) {
// 	do_arena_reset_destroy("arena.0.destroy", arena_ind);
// }



// static bool
// arena_i_initialized(unsigned arena_ind, bool refresh) {
// 	bool initialized;
// 	size_t mib[3];
// 	size_t miblen, sz;

// 	if (refresh) {
// 		uint64_t epoch = 1;
// 		assert_d_eq(mallctl("epoch", NULL, NULL, (void *)&epoch,
// 		    sizeof(epoch)), 0, "Unexpected mallctl() failure");
// 	}

// 	miblen = sizeof(mib)/sizeof(size_t);
// 	assert_d_eq(mallctlnametomib("arena.0.initialized", mib, &miblen), 0,
// 	    "Unexpected mallctlnametomib() failure");
// 	mib[1] = (size_t)arena_ind;
// 	sz = sizeof(initialized);
// 	assert_d_eq(mallctlbymib(mib, miblen, (void *)&initialized, &sz, NULL,
// 	    0), 0, "Unexpected mallctlbymib() failure");

// 	return initialized;
// }


// /*
//  * Actually unmap extents, regardless of opt_retain, so that attempts to access
//  * a destroyed arena's memory will segfault.
//  */
// static bool
// extent_dalloc_unmap(extent_hooks_t *extent_hooks, void *addr, size_t size,
//     bool committed, unsigned arena_ind) {
// 	TRACE_HOOK("%s(extent_hooks=%p, addr=%p, size=%zu, committed=%s, "
// 	    "arena_ind=%u)\n", __func__, extent_hooks, addr, size, committed ?
// 	    "true" : "false", arena_ind);
// 	assert_ptr_eq(extent_hooks, &hooks,
// 	    "extent_hooks should be same as pointer used to set hooks");
// 	assert_ptr_eq(extent_hooks->dalloc, extent_dalloc_unmap,
// 	    "Wrong hook function");
// 	called_dalloc = true;
// 	if (!try_dalloc) {
// 		return true;
// 	}
// 	did_dalloc = true;
// 	if (!maps_coalesce && opt_retain) {
// 		return true;
// 	}
// 	pages_unmap(addr, size);
// 	return false;
// }

// static extent_hooks_t hooks_orig;

// static extent_hooks_t hooks_unmap = {
// 	extent_alloc_hook,
// 	extent_dalloc_unmap, /* dalloc */
// 	extent_destroy_hook,
// 	extent_commit_hook,
// 	extent_decommit_hook,
// 	extent_purge_lazy_hook,
// 	extent_purge_forced_hook,
// 	extent_split_hook,
// 	extent_merge_hook
// };



size_t
arena_decay_backlog_npages_limit(const arena_decay_t *decay);

int test_aaaaa() {
	struct arena_decay_s decaystruct;
	size_t current_npages = 20;
	decaystruct.nunpurged = 10;
	struct arena_decay_s * decay = &decaystruct;
	size_t npages_delta = (current_npages > decay->nunpurged) ?
	    current_npages - decay->nunpurged : 0;
	for (size_t i = 0; i < SMOOTHSTEP_NSTEPS; i++) {
		decay->backlog[i] = 0;
	}
	decay->backlog[SMOOTHSTEP_NSTEPS - 1] = 1;

	size_t npages_limit = arena_decay_backlog_npages_limit(decay);
	printf("npages_limit %lu\n", npages_limit);
	return 0;
}

int
main(void) {
	test_aaaaa();
}

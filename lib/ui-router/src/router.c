#include "private.h"
#include <ui_router.h>

static dict_t *routers = NULL;
static dict_type_t routers_dict_type;

router_t *router_create(const char *name)
{
	router_t *router;

	router = malloc(sizeof(router_t));
	if (!name) {
		name = "default";
	}
	router->name = strdup(name);
	router->link_active_class = strdup("router-link-active");
	router->link_exact_active_class = strdup("router-link-exact-active");
	router->matcher = router_matcher_create();
	router->history = router_history_create();
	if (!routers) {
		dict_init_string_key_type(&routers_dict_type);
		routers = dict_create(&routers_dict_type, NULL);
	}
	dict_add(routers, router->name, router);
	return router;
}

void router_destroy(router_t *router)
{
	dict_delete(routers, router->name);
	router_mem_free(router->name);
	router_mem_free(router->link_active_class);
	router_mem_free(router->link_exact_active_class);
	router_matcher_destroy(router->matcher);
	router_history_destroy(router->history);
	router->matcher = NULL;
	free(router);
}

router_route_record_t *router_add_route_record(
    router_t *router, const router_config_t *config,
    const router_route_record_t *parent)
{
	return router_matcher_add_route_record(router->matcher, config, parent);
}

router_route_t *router_match(router_t *router,
			     const router_location_t *raw_location,
			     const router_route_t *current_route)
{
	return router_matcher_match(router->matcher, raw_location,
				    current_route);
}

router_route_record_t *router_get_matched_route_record(router_t *router,
						       size_t index)
{
	return list_get(&router->history->current->matched, index);
}

router_history_t *router_get_history(router_t *router)
{
	return router->history;
}

router_watcher_t *router_watch(router_t *router, router_callback_t callback,
			       void *data)
{
	return router_history_watch(router->history, callback, data);
}

void router_unwatch(router_t *router, router_watcher_t *watcher)
{
	router_history_unwatch(router->history, watcher);
}

// https://github.com/vuejs/vue-router/blob/65de048ee9f0ebf899ae99c82b71ad397727e55d/dist/vue-router.esm.js#L2827

router_resolved_t *router_resolve(router_t *router, router_location_t *location,
				  bool append)
{
	const router_route_t *current;
	router_resolved_t *resolved;

	current = router->history->current;
	resolved = malloc(sizeof(router_resolved_t));
	resolved->location =
	    router_location_normalize(location, current, append);
	resolved->route = router_match(router, resolved->location, current);
	return resolved;
}

router_location_t *router_resolved_get_location(router_resolved_t *resolved)
{
	return resolved->location;
}

router_route_t *router_resolved_get_route(router_resolved_t *resolved)
{
	return resolved->route;
}

void router_resolved_destroy(router_resolved_t *resolved)
{
	router_location_destroy(resolved->location);
	if (resolved->route) {
		router_route_destroy(resolved->route);
	}
	free(resolved);
}

const router_route_t *router_get_current_route(router_t *router)
{
	return router->history->current;
}

void router_push(router_t *router, router_location_t *location)
{
	router_resolved_t *resolved;

	resolved = router_resolve(router, location, FALSE);
	router_history_push(router->history, resolved->route);
	resolved->route = NULL;
	router_resolved_destroy(resolved);
}

void router_replace(router_t *router, router_location_t *location)
{
	router_resolved_t *resolved;

	resolved = router_resolve(router, location, FALSE);
	router_history_replace(router->history, resolved->route);
	resolved->route = NULL;
	router_resolved_destroy(resolved);
}

void router_go(router_t *router, int delta)
{
	router_history_go(router->history, delta);
}

void router_back(router_t *router)
{
	router_history_go(router->history, -1);
}

void router_forward(router_t *router)
{
	router_history_go(router->history, 1);
}

router_t *router_get_by_name(const char *name)
{
	router_t *router;

	if (!routers) {
		return NULL;
	}
	router = dict_fetch_value(routers, name);
	if (router) {
		return router;
	}
	logger_error("no router named \"%s\" was found", name);
	return NULL;
}

const char *router_get_version(void)
{
	return LIBUI_ROUTER_VERSION;
}

void sofia_handle_sip_r_bye(switch_core_session_t *session, int status,
							char const *phrase,
							nua_t *nua, sofia_profile_t *profile, nua_handle_t *nh, sofia_private_t *sofia_private, sip_t const *sip,
								sofia_dispatch_event_t *de, tagi_t tags[])
{
	if (profile && sofia_test_pflag(profile, PFLAG_FIRE_BYE_RESPONSE_EVENTS) && sip && sip->sip_call_id && !zstr(sip->sip_call_id->i_id) && sofia_private && !zstr_buf(sofia_private->uuid_str)) {
		switch_event_t *bye_response_event = NULL;
		sip_unknown_t *un;
		if (switch_event_create_subclass(&bye_response_event, SWITCH_EVENT_CUSTOM, MY_EVENT_BYE_RESPONSE) == SWITCH_STATUS_SUCCESS) {
			switch_event_add_header(bye_response_event, SWITCH_STACK_BOTTOM, "call-id", "%s", sip->sip_call_id->i_id);
			switch_event_add_header(bye_response_event, SWITCH_STACK_BOTTOM, "Unique-ID", "%s", sofia_private->uuid_str);
			for (un = sip->sip_unknown; un; un = un->un_next) {
				if (!zstr(un->un_value)) {
					switch_event_add_header(bye_response_event, SWITCH_STACK_BOTTOM, un->un_name, "%s", un->un_value);
				}
			}
			switch_event_fire(&bye_response_event);
		}
	}
}
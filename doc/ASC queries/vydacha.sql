# выдача устройства: от нажатия кнопки "Выдать" в карточке ремонта, до закрытия диалогового окна выдачи и обновления элементов на вкладке карточки ремонта.
# РЕМОНТ БЕЗ ЗАПЧАСТЕЙ
# среди запростов есть не относящиеся непосредственно к выдаче, например, уведомления.

SELECT
`Limit2`.`machine_name`
FROM (SELECT
`Limit1`.`machine_name`
FROM (SELECT
1 AS `X`) AS `SingleRowTable1` LEFT OUTER JOIN (SELECT
`Project1`.`id`, 
`Project1`.`machine_name`
FROM `users_activity` AS `Project1`
 WHERE (`Project1`.`user_id` = 32) AND ('Выполнен вход в систему' = `Project1`.`notes`)
 ORDER BY 
`Project1`.`id` DESC LIMIT 1) AS `Limit1` ON 1 = 1 LIMIT 1) AS `Limit2`;

SELECT
`GroupBy1`.`A1` AS `C1`
FROM (SELECT
COUNT(1) AS `A1`
FROM `hooks` AS `Extent1`
 WHERE ((1 = `Extent1`.`hook_id`) AND ('NOTIFY_INTERNAL' = `Extent1`.`event`)) AND ((`Extent1`.`p0` = 6032) OR ((`Extent1`.`p0` IS  NULL) AND (6032 IS  NULL)))) AS `GroupBy1`;

SELECT
`GroupBy1`.`A1` AS `C1`
FROM (SELECT
COUNT(1) AS `A1`
FROM `hooks` AS `Extent1`
 WHERE ((1 = `Extent1`.`hook_id`) AND ('NOTIFY_INTERNAL' = `Extent1`.`event`)) AND ((`Extent1`.`p0` = 6032) OR ((`Extent1`.`p0` IS  NULL) AND (6032 IS  NULL)))) AS `GroupBy1`;

SELECT
`GroupBy1`.`A1` AS `C1`
FROM (SELECT
COUNT(1) AS `A1`
FROM `notifications` AS `Extent1`
 WHERE (`Extent1`.`status` != 1) AND (`Extent1`.`employee` = 32)) AS `GroupBy1`;

SELECT UTC_TIMESTAMP() FROM DUAL;

SELECT
`Extent1`.`id`, 
`Extent1`.`hook_id`, 
`Extent1`.`status`, 
`Extent1`.`created_at`, 
`Extent1`.`updated_at`, 
`Extent1`.`event`, 
`Extent1`.`p0`, 
`Extent1`.`prm`, 
`Extent1`.`task_id`
FROM `hooks` AS `Extent1`
 WHERE ((3 = `Extent1`.`hook_id`) AND ((`Extent1`.`event` IN ( 'NEW_ORDER','ORDER_CHANGED','NEW_CUSTOMER' )) AND (`Extent1`.`event` IS NOT NULL))) AND ((1 = `Extent1`.`status`) OR ((2 = `Extent1`.`status`) AND ('2022-03-23 12:57:36' > `Extent1`.`updated_at`)));

SELECT
`GroupBy1`.`A1` AS `C1`
FROM (SELECT
COUNT(1) AS `A1`
FROM `hooks` AS `Extent1`
 WHERE ((1 = `Extent1`.`hook_id`) AND ('NOTIFY_INTERNAL' = `Extent1`.`event`)) AND ((`Extent1`.`p0` = 6032) OR ((`Extent1`.`p0` IS  NULL) AND (6032 IS  NULL)))) AS `GroupBy1`;

SELECT
`Project2`.`id`, `Project2`.`client`, `Project2`.`type`, `Project2`.`maker`, `Project2`.`model`, `Project2`.`serial_number`, `Project2`.`company`, `Project2`.`office`, 
`Project2`.`start_office`, `Project2`.`manager`, `Project2`.`current_manager`, `Project2`.`master`, `Project2`.`diagnostic_result`, `Project2`.`in_date`, `Project2`.`out_date`, 
`Project2`.`state`, `Project2`.`user_lock`, `Project2`.`lock_datetime`, `Project2`.`express_repair`, `Project2`.`is_warranty`, `Project2`.`is_repeat`, `Project2`.`can_format`, 
`Project2`.`print_check`, `Project2`.`box`, `Project2`.`warranty_label`, `Project2`.`ext_notes`, `Project2`.`is_prepaid`, `Project2`.`prepaid_type`, `Project2`.`prepaid_summ`, 
`Project2`.`prepaid_order`, `Project2`.`is_pre_agreed`, `Project2`.`pre_agreed_amount`, `Project2`.`repair_cost`, `Project2`.`fault`, `Project2`.`complect`, `Project2`.`look`, 
`Project2`.`thirs_party_sc`, `Project2`.`last_save`, `Project2`.`last_status_changed`, `Project2`.`warranty_days`, `Project2`.`barcode`, `Project2`.`new_state`, `Project2`.`is_card_payment`, 
`Project2`.`informed_status`, `Project2`.`parts_cost`, `Project2`.`is_debt`, `Project2`.`image_ids`, `Project2`.`color`, `Project2`.`order_moving`, `Project2`.`payment_system`, 
`Project2`.`early`, `Project2`.`real_repair_cost`, `Project2`.`issued_msg`, `Project2`.`reject_reason`, `Project2`.`ext_early`, `Project2`.`sms_inform`, `Project2`.`invoice`, 
`Project2`.`cartridge`, `Project2`.`termsControl`, `Project2`.`vendor_id`, `Project2`.`quick_repair`, `Project2`.`Hidden`, `Project2`.`Title`, `Project2`.`id1`, `Project2`.`sip_user_id`, 
`Project2`.`username`, `Project2`.`name`, `Project2`.`surname`, `Project2`.`patronymic`, `Project2`.`phone`, `Project2`.`phone2`, `Project2`.`phone_mask`, `Project2`.`phone2_mask`, 
`Project2`.`address`, `Project2`.`passport_num`, `Project2`.`passport_date`, `Project2`.`passport_organ`, `Project2`.`state1`, `Project2`.`created`, `Project2`.`office1`, 
`Project2`.`birthday`, `Project2`.`last_login`, `Project2`.`last_activity`, `Project2`.`email`, `Project2`.`sex`, `Project2`.`photo`, `Project2`.`salary_rate`, `Project2`.`pay_day`, 
`Project2`.`pay_day_off`, `Project2`.`pay_repair`, `Project2`.`pay_sale`, `Project2`.`pay_repair_q_sale`, `Project2`.`pay_cartridge_refill`, `Project2`.`color_label_ws`, 
`Project2`.`workspace_mode`, `Project2`.`preview_before_print`, `Project2`.`new_rep_doc_copies`, `Project2`.`auto_refresh_workspace`, `Project2`.`refresh_time`, `Project2`.`xls_c1`, 
`Project2`.`xls_c2`, `Project2`.`xls_c3`, `Project2`.`xls_c4`, `Project2`.`xls_c5`, `Project2`.`xls_c6`, `Project2`.`xls_c7`, `Project2`.`xls_c8`, `Project2`.`xls_c9`, `Project2`.`xls_c10`, 
`Project2`.`xls_c11`, `Project2`.`xls_c12`, `Project2`.`xls_c13`, `Project2`.`xls_c14`, `Project2`.`xls_c15`, `Project2`.`display_out`, `Project2`.`display_complete`, `Project2`.`is_bot`, 
`Project2`.`new_on_top`, `Project2`.`issued_color`, `Project2`.`fields_cfg`, `Project2`.`kkm_pass`, `Project2`.`prefer_regular`, `Project2`.`fontsize`, `Project2`.`rowheight`, 
`Project2`.`animation`, `Project2`.`pay_device_in`, `Project2`.`pay_device_out`, `Project2`.`pay_4_sale_in_repair`, `Project2`.`row_color`, `Project2`.`save_state_on_close`, 
`Project2`.`group_store_items`, `Project2`.`track_activity`, `Project2`.`inn`, `Project2`.`inform_comment`, `Project2`.`inform_status`, `Project2`.`kkt`, `Project2`.`pinpad`, 
`Project2`.`def_office`, `Project2`.`def_store`, `Project2`.`def_item_state`, `Project2`.`def_employee`, `Project2`.`def_status`, `Project2`.`def_ws_filter`, `Project2`.`card_on_call`, 
`Project2`.`ge_highlight_color`, `Project2`.`pay_repair_quick`, `Project2`.`advance_disable`, `Project2`.`salary_disable`, `Project2`.`notes`, `Project2`.`id2`, `Project2`.`creator`, 
`Project2`.`name1`, `Project2`.`surname1`, `Project2`.`patronymic1`, `Project2`.`agent_phone_mask`, `Project2`.`agent2_phone_mask`, `Project2`.`address1`, `Project2`.`post_index`, 
`Project2`.`passport_num1`, `Project2`.`passport_date1`, `Project2`.`passport_organ1`, `Project2`.`state2`, `Project2`.`type1`, `Project2`.`birthday1`, `Project2`.`memorial`, 
`Project2`.`notes1`, `Project2`.`is_regular`, `Project2`.`is_dealer`, `Project2`.`balance_enable`, `Project2`.`prefer_cashless`, `Project2`.`take_long`, `Project2`.`ignore_calls`, 
`Project2`.`is_bad`, `Project2`.`is_realizator`, `Project2`.`is_agent`, `Project2`.`inn1`, `Project2`.`KPP`, `Project2`.`OGRN`, `Project2`.`web_password`, `Project2`.`ur_name`, 
`Project2`.`email1`, `Project2`.`icq`, `Project2`.`skype`, `Project2`.`viber`, `Project2`.`telegram`, `Project2`.`site`, `Project2`.`whatsapp`, `Project2`.`agent_name`, 
`Project2`.`agent_surname`, `Project2`.`agent_patronymic`, `Project2`.`agent_phone`, `Project2`.`agent2_name`, `Project2`.`agent2_surname`, `Project2`.`agent2_patronymic`, 
`Project2`.`agent2_phone`, `Project2`.`created1`, `Project2`.`balance`, `Project2`.`price_col`, `Project2`.`agent_phone_clean`, `Project2`.`agent2_phone_clean`, `Project2`.`repairs`, 
`Project2`.`token`, `Project2`.`purchases`, `Project2`.`visit_source`, `Project2`.`photo_id`, `Project2`.`id3`, `Project2`.`num`, `Project2`.`created2`, `Project2`.`user`, 
`Project2`.`seller`, `Project2`.`customer`, `Project2`.`paid`, `Project2`.`tax`, `Project2`.`summ`, `Project2`.`total`, `Project2`.`state3`, `Project2`.`office2`, `Project2`.`type2`, 
`Project2`.`notes2`, `Project2`.`id4`, `Project2`.`refill`, `Project2`.`chip`, `Project2`.`opc_drum`, `Project2`.`c_blade`, `Project2`.`card_id`, `Project2`.`id5`, `Project2`.`name2`, 
`Project2`.`maker1`, `Project2`.`full_weight`, `Project2`.`toner_weight`, `Project2`.`resource`, `Project2`.`created3`, `Project2`.`user1`, `Project2`.`notes3`, `Project2`.`photo1`, 
`Project2`.`color1`, `Project2`.`archive`, `Project2`.`C1`, `Project2`.`id6`, `Project2`.`field_id`, `Project2`.`repair_id`, `Project2`.`value`, `Project2`.`item_id`, `Project2`.`id7`, 
`Project2`.`name3`, `Project2`.`type3`, `Project2`.`def_values`, `Project2`.`required`, `Project2`.`printable`, `Project2`.`archive1`, `Project2`.`_f`, `Project2`.`updated_at`, 
`Project2`.`devices`, `Project2`.`categories`
FROM (SELECT
`Limit1`.`id`, `Limit1`.`Hidden`, `Limit1`.`Title`, `Limit1`.`client`, `Limit1`.`type`, `Limit1`.`maker`, `Limit1`.`model`, `Limit1`.`serial_number`, `Limit1`.`company`, `Limit1`.`office`, 
`Limit1`.`start_office`, `Limit1`.`manager`, `Limit1`.`current_manager`, `Limit1`.`master`, `Limit1`.`diagnostic_result`, `Limit1`.`in_date`, `Limit1`.`out_date`, `Limit1`.`state`, 
`Limit1`.`new_state`, `Limit1`.`user_lock`, `Limit1`.`lock_datetime`, `Limit1`.`express_repair`, `Limit1`.`quick_repair`, `Limit1`.`is_warranty`, `Limit1`.`is_repeat`, 
`Limit1`.`payment_system`, `Limit1`.`is_card_payment`, `Limit1`.`can_format`, `Limit1`.`print_check`, `Limit1`.`box`, `Limit1`.`warranty_label`, `Limit1`.`ext_notes`, `Limit1`.`is_prepaid`, 
`Limit1`.`prepaid_type`, `Limit1`.`prepaid_summ`, `Limit1`.`prepaid_order`, `Limit1`.`is_pre_agreed`, `Limit1`.`is_debt`, `Limit1`.`pre_agreed_amount`, `Limit1`.`repair_cost`, 
`Limit1`.`real_repair_cost`, `Limit1`.`parts_cost`, `Limit1`.`fault`, `Limit1`.`complect`, `Limit1`.`look`, `Limit1`.`thirs_party_sc`, `Limit1`.`last_save`, `Limit1`.`last_status_changed`, 
`Limit1`.`warranty_days`, `Limit1`.`barcode`, `Limit1`.`reject_reason`, `Limit1`.`informed_status`, `Limit1`.`image_ids`, `Limit1`.`color`, `Limit1`.`order_moving`, `Limit1`.`early`, 
`Limit1`.`ext_early`, `Limit1`.`issued_msg`, `Limit1`.`sms_inform`, `Limit1`.`invoice`, `Limit1`.`cartridge`, `Limit1`.`vendor_id`, `Limit1`.`termsControl`, `Limit1`.`id1`, 
`Limit1`.`sip_user_id`, `Limit1`.`username`, `Limit1`.`name`, `Limit1`.`surname`, `Limit1`.`patronymic`, `Limit1`.`phone`, `Limit1`.`phone2`, `Limit1`.`phone_mask`, `Limit1`.`phone2_mask`, 
`Limit1`.`address`, `Limit1`.`passport_num`, `Limit1`.`passport_date`, `Limit1`.`passport_organ`, `Limit1`.`state1`, `Limit1`.`created`, `Limit1`.`office1`, `Limit1`.`birthday`, 
`Limit1`.`def_office`, `Limit1`.`def_store`, `Limit1`.`def_item_state`, `Limit1`.`def_employee`, `Limit1`.`def_status`, `Limit1`.`def_ws_filter`, `Limit1`.`last_login`, 
`Limit1`.`last_activity`, `Limit1`.`email`, `Limit1`.`sex`, `Limit1`.`photo`, `Limit1`.`salary_rate`, `Limit1`.`pay_day`, `Limit1`.`pay_day_off`, `Limit1`.`pay_repair`, 
`Limit1`.`pay_repair_quick`, `Limit1`.`pay_sale`, `Limit1`.`pay_repair_q_sale`, `Limit1`.`pay_cartridge_refill`, `Limit1`.`pay_device_in`, `Limit1`.`pay_device_out`, 
`Limit1`.`pay_4_sale_in_repair`, `Limit1`.`row_color`, `Limit1`.`ge_highlight_color`, `Limit1`.`color_label_ws`, `Limit1`.`workspace_mode`, `Limit1`.`preview_before_print`, 
`Limit1`.`new_rep_doc_copies`, `Limit1`.`auto_refresh_workspace`, `Limit1`.`refresh_time`, `Limit1`.`xls_c1`, `Limit1`.`xls_c2`, `Limit1`.`xls_c3`, `Limit1`.`xls_c4`, `Limit1`.`xls_c5`, 
`Limit1`.`xls_c6`, `Limit1`.`xls_c7`, `Limit1`.`xls_c8`, `Limit1`.`xls_c9`, `Limit1`.`xls_c10`, `Limit1`.`xls_c11`, `Limit1`.`xls_c12`, `Limit1`.`xls_c13`, `Limit1`.`xls_c14`, 
`Limit1`.`xls_c15`, `Limit1`.`display_out`, `Limit1`.`display_complete`, `Limit1`.`is_bot`, `Limit1`.`new_on_top`, `Limit1`.`issued_color`, `Limit1`.`kkm_pass`, `Limit1`.`prefer_regular`, 
`Limit1`.`fontsize`, `Limit1`.`rowheight`, `Limit1`.`animation`, `Limit1`.`fields_cfg`, `Limit1`.`save_state_on_close`, `Limit1`.`group_store_items`, `Limit1`.`track_activity`, 
`Limit1`.`card_on_call`, `Limit1`.`inn`, `Limit1`.`inform_comment`, `Limit1`.`inform_status`, `Limit1`.`kkt`, `Limit1`.`pinpad`, `Limit1`.`advance_disable`, `Limit1`.`salary_disable`, 
`Limit1`.`notes`, `Limit1`.`id2`, `Limit1`.`creator`, `Limit1`.`name1`, `Limit1`.`surname1`, `Limit1`.`patronymic1`, `Limit1`.`agent_phone_mask`, `Limit1`.`agent2_phone_mask`, 
`Limit1`.`address1`, `Limit1`.`post_index`, `Limit1`.`passport_num1`, `Limit1`.`passport_date1`, `Limit1`.`passport_organ1`, `Limit1`.`state2`, `Limit1`.`type1`, `Limit1`.`birthday1`, 
`Limit1`.`memorial`, `Limit1`.`notes1`, `Limit1`.`is_regular`, `Limit1`.`is_dealer`, `Limit1`.`balance_enable`, `Limit1`.`prefer_cashless`, `Limit1`.`take_long`, `Limit1`.`ignore_calls`, 
`Limit1`.`is_bad`, `Limit1`.`is_realizator`, `Limit1`.`is_agent`, `Limit1`.`visit_source`, `Limit1`.`photo_id`, `Limit1`.`inn1`, `Limit1`.`KPP`, `Limit1`.`OGRN`, `Limit1`.`web_password`, 
`Limit1`.`ur_name`, `Limit1`.`email1`, `Limit1`.`icq`, `Limit1`.`skype`, `Limit1`.`viber`, `Limit1`.`telegram`, `Limit1`.`site`, `Limit1`.`whatsapp`, `Limit1`.`agent_name`, 
`Limit1`.`agent_surname`, `Limit1`.`agent_patronymic`, `Limit1`.`agent_phone`, `Limit1`.`agent_phone_clean`, `Limit1`.`agent2_name`, `Limit1`.`agent2_surname`, `Limit1`.`agent2_patronymic`, 
`Limit1`.`agent2_phone`, `Limit1`.`agent2_phone_clean`, `Limit1`.`created1`, `Limit1`.`balance`, `Limit1`.`price_col`, `Limit1`.`repairs`, `Limit1`.`purchases`, `Limit1`.`token`, 
`Limit1`.`id3`, `Limit1`.`num`, `Limit1`.`created2`, `Limit1`.`user`, `Limit1`.`seller`, `Limit1`.`customer`, `Limit1`.`paid`, `Limit1`.`tax`, `Limit1`.`summ`, `Limit1`.`total`, 
`Limit1`.`state3`, `Limit1`.`office2`, `Limit1`.`type2`, `Limit1`.`notes2`, `Limit1`.`id4`, `Limit1`.`refill`, `Limit1`.`chip`, `Limit1`.`opc_drum`, `Limit1`.`c_blade`, `Limit1`.`card_id`, 
`Limit1`.`id5`, `Limit1`.`name2`, `Limit1`.`maker1`, `Limit1`.`full_weight`, `Limit1`.`toner_weight`, `Limit1`.`resource`, `Limit1`.`created3`, `Limit1`.`user1`, `Limit1`.`notes3`, 
`Limit1`.`photo1`, `Limit1`.`color1`, `Limit1`.`archive`, `Join6`.`id` AS `id6`, `Join6`.`field_id`, `Join6`.`repair_id`, `Join6`.`item_id`, `Join6`.`value`, `Join6`.`ID1` AS `id7`, 
`Join6`.`_f`, `Join6`.`name` AS `name3`, `Join6`.`type` AS `type3`, `Join6`.`def_values`, `Join6`.`required`, `Join6`.`printable`, `Join6`.`devices`, `Join6`.`categories`, 
`Join6`.`updated_at`, `Join6`.`archive` AS `archive1`, CASE WHEN (`Join6`.`id` IS NOT NULL) THEN (1)  ELSE (NULL) END AS `C1`
FROM (SELECT
`Extent1`.`id`, `Extent1`.`Hidden`, `Extent1`.`Title`, `Extent1`.`client`, `Extent1`.`type`, `Extent1`.`maker`, `Extent1`.`model`, `Extent1`.`serial_number`, `Extent1`.`company`, 
`Extent1`.`office`, `Extent1`.`start_office`, `Extent1`.`manager`, `Extent1`.`current_manager`, `Extent1`.`master`, `Extent1`.`diagnostic_result`, `Extent1`.`in_date`, `Extent1`.`out_date`, 
`Extent1`.`state`, `Extent1`.`new_state`, `Extent1`.`user_lock`, `Extent1`.`lock_datetime`, `Extent1`.`express_repair`, `Extent1`.`quick_repair`, `Extent1`.`is_warranty`, 
`Extent1`.`is_repeat`, `Extent1`.`payment_system`, `Extent1`.`is_card_payment`, `Extent1`.`can_format`, `Extent1`.`print_check`, `Extent1`.`box`, `Extent1`.`warranty_label`, 
`Extent1`.`ext_notes`, `Extent1`.`is_prepaid`, `Extent1`.`prepaid_type`, `Extent1`.`prepaid_summ`, `Extent1`.`prepaid_order`, `Extent1`.`is_pre_agreed`, `Extent1`.`is_debt`, 
`Extent1`.`pre_agreed_amount`, `Extent1`.`repair_cost`, `Extent1`.`real_repair_cost`, `Extent1`.`parts_cost`, `Extent1`.`fault`, `Extent1`.`complect`, `Extent1`.`look`, 
`Extent1`.`thirs_party_sc`, `Extent1`.`last_save`, `Extent1`.`last_status_changed`, `Extent1`.`warranty_days`, `Extent1`.`barcode`, `Extent1`.`reject_reason`, `Extent1`.`informed_status`, 
`Extent1`.`image_ids`, `Extent1`.`color`, `Extent1`.`order_moving`, `Extent1`.`early`, `Extent1`.`ext_early`, `Extent1`.`issued_msg`, `Extent1`.`sms_inform`, `Extent1`.`invoice`, 
`Extent1`.`cartridge`, `Extent1`.`vendor_id`, `Extent1`.`termsControl`, `Extent2`.`id` AS `id1`, `Extent2`.`sip_user_id`, `Extent2`.`username`, `Extent2`.`name`, `Extent2`.`surname`, 
`Extent2`.`patronymic`, `Extent2`.`phone`, `Extent2`.`phone2`, `Extent2`.`phone_mask`, `Extent2`.`phone2_mask`, `Extent2`.`address`, `Extent2`.`passport_num`, `Extent2`.`passport_date`, 
`Extent2`.`passport_organ`, `Extent2`.`state` AS `state1`, `Extent2`.`created`, `Extent2`.`office` AS `office1`, `Extent2`.`birthday`, `Extent2`.`def_office`, `Extent2`.`def_store`, 
`Extent2`.`def_item_state`, `Extent2`.`def_employee`, `Extent2`.`def_status`, `Extent2`.`def_ws_filter`, `Extent2`.`last_login`, `Extent2`.`last_activity`, `Extent2`.`email`, 
`Extent2`.`sex`, `Extent2`.`photo`, `Extent2`.`salary_rate`, `Extent2`.`pay_day`, `Extent2`.`pay_day_off`, `Extent2`.`pay_repair`, `Extent2`.`pay_repair_quick`, `Extent2`.`pay_sale`, 
`Extent2`.`pay_repair_q_sale`, `Extent2`.`pay_cartridge_refill`, `Extent2`.`pay_device_in`, `Extent2`.`pay_device_out`, `Extent2`.`pay_4_sale_in_repair`, `Extent2`.`row_color`, 
`Extent2`.`ge_highlight_color`, `Extent2`.`color_label_ws`, `Extent2`.`workspace_mode`, `Extent2`.`preview_before_print`, `Extent2`.`new_rep_doc_copies`, `Extent2`.`auto_refresh_workspace`, 
`Extent2`.`refresh_time`, `Extent2`.`xls_c1`, `Extent2`.`xls_c2`, `Extent2`.`xls_c3`, `Extent2`.`xls_c4`, `Extent2`.`xls_c5`, `Extent2`.`xls_c6`, `Extent2`.`xls_c7`, `Extent2`.`xls_c8`, 
`Extent2`.`xls_c9`, `Extent2`.`xls_c10`, `Extent2`.`xls_c11`, `Extent2`.`xls_c12`, `Extent2`.`xls_c13`, `Extent2`.`xls_c14`, `Extent2`.`xls_c15`, `Extent2`.`display_out`, 
`Extent2`.`display_complete`, `Extent2`.`is_bot`, `Extent2`.`new_on_top`, `Extent2`.`issued_color`, `Extent2`.`kkm_pass`, `Extent2`.`prefer_regular`, `Extent2`.`fontsize`, 
`Extent2`.`rowheight`, `Extent2`.`animation`, `Extent2`.`fields_cfg`, `Extent2`.`save_state_on_close`, `Extent2`.`group_store_items`, `Extent2`.`track_activity`, `Extent2`.`card_on_call`, 
`Extent2`.`inn`, `Extent2`.`inform_comment`, `Extent2`.`inform_status`, `Extent2`.`kkt`, `Extent2`.`pinpad`, `Extent2`.`advance_disable`, `Extent2`.`salary_disable`, `Extent2`.`notes`, 
`Extent3`.`id` AS `id2`, `Extent3`.`creator`, `Extent3`.`name` AS `name1`, `Extent3`.`surname` AS `surname1`, `Extent3`.`patronymic` AS `patronymic1`, `Extent3`.`agent_phone_mask`, 
`Extent3`.`agent2_phone_mask`, `Extent3`.`address` AS `address1`, `Extent3`.`post_index`, `Extent3`.`passport_num` AS `passport_num1`, `Extent3`.`passport_date` AS `passport_date1`, 
`Extent3`.`passport_organ` AS `passport_organ1`, `Extent3`.`state` AS `state2`, `Extent3`.`type` AS `type1`, `Extent3`.`birthday` AS `birthday1`, `Extent3`.`memorial`, `Extent3`.`notes` AS 
`notes1`, `Extent3`.`is_regular`, `Extent3`.`is_dealer`, `Extent3`.`balance_enable`, `Extent3`.`prefer_cashless`, `Extent3`.`take_long`, `Extent3`.`ignore_calls`, `Extent3`.`is_bad`, 
`Extent3`.`is_realizator`, `Extent3`.`is_agent`, `Extent3`.`visit_source`, `Extent3`.`photo_id`, `Extent3`.`INN` AS `inn1`, `Extent3`.`KPP`, `Extent3`.`OGRN`, `Extent3`.`web_password`, 
`Extent3`.`ur_name`, `Extent3`.`email` AS `email1`, `Extent3`.`icq`, `Extent3`.`skype`, `Extent3`.`viber`, `Extent3`.`telegram`, `Extent3`.`site`, `Extent3`.`whatsapp`, 
`Extent3`.`agent_name`, `Extent3`.`agent_surname`, `Extent3`.`agent_patronymic`, `Extent3`.`agent_phone`, `Extent3`.`agent_phone_clean`, `Extent3`.`agent2_name`, `Extent3`.`agent2_surname`, 
`Extent3`.`agent2_patronymic`, `Extent3`.`agent2_phone`, `Extent3`.`agent2_phone_clean`, `Extent3`.`created` AS `created1`, `Extent3`.`balance`, `Extent3`.`price_col`, `Extent3`.`repairs`, 
`Extent3`.`purchases`, `Extent3`.`token`, `Extent4`.`id` AS `id3`, `Extent4`.`num`, `Extent4`.`created` AS `created2`, `Extent4`.`user`, `Extent4`.`seller`, `Extent4`.`customer`, 
`Extent4`.`paid`, `Extent4`.`tax`, `Extent4`.`summ`, `Extent4`.`total`, `Extent4`.`state` AS `state3`, `Extent4`.`office` AS `office2`, `Extent4`.`type` AS `type2`, `Extent4`.`notes` AS 
`notes2`, `Extent5`.`id` AS `id4`, `Extent5`.`refill`, `Extent5`.`chip`, `Extent5`.`opc_drum`, `Extent5`.`c_blade`, `Extent5`.`card_id`, `Extent6`.`id` AS `id5`, `Extent6`.`name` AS `name2`, 
`Extent6`.`maker` AS `maker1`, `Extent6`.`full_weight`, `Extent6`.`toner_weight`, `Extent6`.`resource`, `Extent6`.`created` AS `created3`, `Extent6`.`user` AS `user1`, `Extent6`.`notes` AS 
`notes3`, `Extent6`.`photo` AS `photo1`, `Extent6`.`color` AS `color1`, `Extent6`.`archive`
FROM `workshop` AS `Extent1` LEFT OUTER JOIN `users` AS `Extent2` ON `Extent1`.`user_lock` = `Extent2`.`id` INNER JOIN `clients` AS `Extent3` ON `Extent1`.`client` = `Extent3`.`id` LEFT OUTER JOIN `invoice` AS `Extent4` ON `Extent1`.`invoice` = `Extent4`.`id` LEFT OUTER JOIN `c_workshop` AS `Extent5` ON `Extent1`.`cartridge` = `Extent5`.`id` LEFT OUTER JOIN `cartridge_cards` AS `Extent6` ON `Extent5`.`card_id` = `Extent6`.`id`
 WHERE `Extent1`.`id` = 24903 LIMIT 2) AS `Limit1` LEFT OUTER JOIN (SELECT
`Extent7`.`id`, `Extent7`.`field_id`, `Extent7`.`repair_id`, `Extent7`.`item_id`, `Extent7`.`value`, `Extent8`.`id` AS `ID1`, `Extent8`.`_f`, `Extent8`.`name`, `Extent8`.`type`, 
`Extent8`.`def_values`, `Extent8`.`required`, `Extent8`.`printable`, `Extent8`.`devices`, `Extent8`.`categories`, `Extent8`.`updated_at`, `Extent8`.`archive`
FROM `field_values` AS `Extent7` INNER JOIN `fields` AS `Extent8` ON `Extent7`.`field_id` = `Extent8`.`id`) AS `Join6` ON `Limit1`.`id` = `Join6`.`repair_id`) AS `Project2`
 ORDER BY 
`Project2`.`id` ASC, `Project2`.`id1` ASC, `Project2`.`id2` ASC, `Project2`.`id3` ASC, `Project2`.`id4` ASC, `Project2`.`id5` ASC, `Project2`.`C1` ASC;

BEGIN;
UPDATE `workshop` SET `user_lock`=32, `lock_datetime`='2022-03-23 13:07:37.911408' WHERE `id` = 24903;
COMMIT;

SELECT
`GroupBy1`.`A1` AS `C1`
FROM (SELECT
SUM(CASE WHEN (`Project1`.`C1` IS NOT NULL) THEN (`Project1`.`summa`)  ELSE (0) END) AS `A1`
FROM (SELECT
1 AS `X`) AS `SingleRowTable1` LEFT OUTER JOIN (SELECT
`Extent1`.`summa`, 
1 AS `C1`
FROM `cash_orders` AS `Extent1`
 WHERE `Extent1`.`repair` = 24903) AS `Project1` ON 1 = 1) AS `GroupBy1`;

SELECT
`GroupBy1`.`A1` AS `C1`
FROM (SELECT
SUM(CASE WHEN (`Project1`.`C2` IS NOT NULL) THEN (`Project1`.`C1`)  ELSE (0) END) AS `A1`
FROM (SELECT
1 AS `X`) AS `SingleRowTable1` LEFT OUTER JOIN (SELECT
`Extent1`.`price` * (`Extent1`.`count`) AS `C1`, 
1 AS `C2`
FROM `works` AS `Extent1`
 WHERE `Extent1`.`repair` = 24903) AS `Project1` ON 1 = 1) AS `GroupBy1`;

SELECT
`GroupBy1`.`A1` AS `C1`
FROM (SELECT
SUM(CASE WHEN (`Project1`.`C2` IS NOT NULL) THEN (`Project1`.`C1`)  ELSE (0) END) AS `A1`
FROM (SELECT
1 AS `X`) AS `SingleRowTable1` LEFT OUTER JOIN (SELECT
(`Extent1`.`count`) * `Extent1`.`price` AS `C1`, 
1 AS `C2`
FROM `store_int_reserve` AS `Extent1`
 WHERE (`Extent1`.`repair_id` = 24903) AND ((2 = `Extent1`.`state`) OR (3 = `Extent1`.`state`))) AS `Project1` ON 1 = 1) AS `GroupBy1`;

SELECT
`GroupBy1`.`A1` AS `C1`
FROM (SELECT
SUM(CASE WHEN (`Project1`.`C1` IS NOT NULL) THEN (`Project1`.`summa`)  ELSE (0) END) AS `A1`
FROM (SELECT
1 AS `X`) AS `SingleRowTable1` LEFT OUTER JOIN (SELECT
`Extent1`.`summa`, 
1 AS `C1`
FROM `cash_orders` AS `Extent1`
 WHERE `Extent1`.`repair` = 24903) AS `Project1` ON 1 = 1) AS `GroupBy1`;

SELECT
`GroupBy1`.`A1` AS `C1`
FROM (SELECT
SUM(CASE WHEN (`Project1`.`C2` IS NOT NULL) THEN (`Project1`.`C1`)  ELSE (0) END) AS `A1`
FROM (SELECT
1 AS `X`) AS `SingleRowTable1` LEFT OUTER JOIN (SELECT
`Extent1`.`price` * (`Extent1`.`count`) AS `C1`, 
1 AS `C2`
FROM `works` AS `Extent1`
 WHERE `Extent1`.`repair` = 24903) AS `Project1` ON 1 = 1) AS `GroupBy1`;

SELECT
`GroupBy1`.`A1` AS `C1`
FROM (SELECT
SUM(CASE WHEN (`Project1`.`C2` IS NOT NULL) THEN (`Project1`.`C1`)  ELSE (0) END) AS `A1`
FROM (SELECT
1 AS `X`) AS `SingleRowTable1` LEFT OUTER JOIN (SELECT
(`Extent1`.`count`) * `Extent1`.`price` AS `C1`, 
1 AS `C2`
FROM `store_int_reserve` AS `Extent1`
 WHERE (`Extent1`.`repair_id` = 24903) AND ((2 = `Extent1`.`state`) OR (3 = `Extent1`.`state`))) AS `Project1` ON 1 = 1) AS `GroupBy1`;

SELECT
`Project2`.`id`, `Project2`.`creator`, `Project2`.`name`, `Project2`.`surname`, `Project2`.`patronymic`, `Project2`.`agent_phone_mask`, `Project2`.`agent2_phone_mask`, `Project2`.`address`, 
`Project2`.`post_index`, `Project2`.`passport_num`, `Project2`.`passport_date`, `Project2`.`passport_organ`, `Project2`.`state`, `Project2`.`type`, `Project2`.`birthday`, 
`Project2`.`memorial`, `Project2`.`notes`, `Project2`.`is_regular`, `Project2`.`is_dealer`, `Project2`.`balance_enable`, `Project2`.`prefer_cashless`, `Project2`.`take_long`, 
`Project2`.`ignore_calls`, `Project2`.`is_bad`, `Project2`.`is_realizator`, `Project2`.`is_agent`, `Project2`.`INN`, `Project2`.`KPP`, `Project2`.`OGRN`, `Project2`.`web_password`, 
`Project2`.`ur_name`, `Project2`.`email`, `Project2`.`icq`, `Project2`.`skype`, `Project2`.`viber`, `Project2`.`telegram`, `Project2`.`site`, `Project2`.`whatsapp`, `Project2`.`agent_name`, 
`Project2`.`agent_surname`, `Project2`.`agent_patronymic`, `Project2`.`agent_phone`, `Project2`.`agent2_name`, `Project2`.`agent2_surname`, `Project2`.`agent2_patronymic`, 
`Project2`.`agent2_phone`, `Project2`.`created`, `Project2`.`balance`, `Project2`.`price_col`, `Project2`.`agent_phone_clean`, `Project2`.`agent2_phone_clean`, `Project2`.`repairs`, 
`Project2`.`token`, `Project2`.`purchases`, `Project2`.`visit_source`, `Project2`.`photo_id`, `Project2`.`C1`, `Project2`.`id1`, `Project2`.`phone`, `Project2`.`phone_clean`, 
`Project2`.`mask`, `Project2`.`customer`, `Project2`.`type1`, `Project2`.`note`
FROM (SELECT
`Limit1`.`id`, `Limit1`.`creator`, `Limit1`.`name`, `Limit1`.`surname`, `Limit1`.`patronymic`, `Limit1`.`agent_phone_mask`, `Limit1`.`agent2_phone_mask`, `Limit1`.`address`, 
`Limit1`.`post_index`, `Limit1`.`passport_num`, `Limit1`.`passport_date`, `Limit1`.`passport_organ`, `Limit1`.`state`, `Limit1`.`type`, `Limit1`.`birthday`, `Limit1`.`memorial`, 
`Limit1`.`notes`, `Limit1`.`is_regular`, `Limit1`.`is_dealer`, `Limit1`.`balance_enable`, `Limit1`.`prefer_cashless`, `Limit1`.`take_long`, `Limit1`.`ignore_calls`, `Limit1`.`is_bad`, 
`Limit1`.`is_realizator`, `Limit1`.`is_agent`, `Limit1`.`visit_source`, `Limit1`.`photo_id`, `Limit1`.`INN`, `Limit1`.`KPP`, `Limit1`.`OGRN`, `Limit1`.`web_password`, `Limit1`.`ur_name`, 
`Limit1`.`email`, `Limit1`.`icq`, `Limit1`.`skype`, `Limit1`.`viber`, `Limit1`.`telegram`, `Limit1`.`site`, `Limit1`.`whatsapp`, `Limit1`.`agent_name`, `Limit1`.`agent_surname`, 
`Limit1`.`agent_patronymic`, `Limit1`.`agent_phone`, `Limit1`.`agent_phone_clean`, `Limit1`.`agent2_name`, `Limit1`.`agent2_surname`, `Limit1`.`agent2_patronymic`, `Limit1`.`agent2_phone`, 
`Limit1`.`agent2_phone_clean`, `Limit1`.`created`, `Limit1`.`balance`, `Limit1`.`price_col`, `Limit1`.`repairs`, `Limit1`.`purchases`, `Limit1`.`token`, `Extent2`.`id` AS `id1`, 
`Extent2`.`phone`, `Extent2`.`phone_clean`, `Extent2`.`mask`, `Extent2`.`customer`, `Extent2`.`type` AS `type1`, `Extent2`.`note`, CASE WHEN (`Extent2`.`id` IS NOT NULL) THEN (1)  ELSE 
(NULL) END AS `C1`
FROM (SELECT
`Extent1`.`id`, `Extent1`.`creator`, `Extent1`.`name`, `Extent1`.`surname`, `Extent1`.`patronymic`, `Extent1`.`agent_phone_mask`, `Extent1`.`agent2_phone_mask`, `Extent1`.`address`, 
`Extent1`.`post_index`, `Extent1`.`passport_num`, `Extent1`.`passport_date`, `Extent1`.`passport_organ`, `Extent1`.`state`, `Extent1`.`type`, `Extent1`.`birthday`, `Extent1`.`memorial`, 
`Extent1`.`notes`, `Extent1`.`is_regular`, `Extent1`.`is_dealer`, `Extent1`.`balance_enable`, `Extent1`.`prefer_cashless`, `Extent1`.`take_long`, `Extent1`.`ignore_calls`, 
`Extent1`.`is_bad`, `Extent1`.`is_realizator`, `Extent1`.`is_agent`, `Extent1`.`visit_source`, `Extent1`.`photo_id`, `Extent1`.`INN`, `Extent1`.`KPP`, `Extent1`.`OGRN`, 
`Extent1`.`web_password`, `Extent1`.`ur_name`, `Extent1`.`email`, `Extent1`.`icq`, `Extent1`.`skype`, `Extent1`.`viber`, `Extent1`.`telegram`, `Extent1`.`site`, `Extent1`.`whatsapp`, 
`Extent1`.`agent_name`, `Extent1`.`agent_surname`, `Extent1`.`agent_patronymic`, `Extent1`.`agent_phone`, `Extent1`.`agent_phone_clean`, `Extent1`.`agent2_name`, `Extent1`.`agent2_surname`, 
`Extent1`.`agent2_patronymic`, `Extent1`.`agent2_phone`, `Extent1`.`agent2_phone_clean`, `Extent1`.`created`, `Extent1`.`balance`, `Extent1`.`price_col`, `Extent1`.`repairs`, 
`Extent1`.`purchases`, `Extent1`.`token`
FROM `clients` AS `Extent1`
 WHERE `Extent1`.`id` = 1285 LIMIT 1) AS `Limit1` LEFT OUTER JOIN `tel` AS `Extent2` ON `Limit1`.`id` = `Extent2`.`customer`) AS `Project2`
 ORDER BY 
`Project2`.`id` ASC, 
`Project2`.`C1` ASC;

SELECT
`Limit1`.`id`, 
`Limit1`.`permission_id`, 
`Limit1`.`role_id`
FROM (SELECT
`Extent1`.`id`, 
`Extent1`.`permission_id`, 
`Extent1`.`role_id`
FROM `permissions_roles` AS `Extent1` INNER JOIN `roles_users` AS `Extent2` ON `Extent1`.`role_id` = `Extent2`.`role_id`
 WHERE (`Extent2`.`user_id` = 32) AND (`Extent1`.`permission_id` = 64) LIMIT 1) AS `Limit1`;

SELECT
`GroupBy1`.`A1` AS `C1`
FROM (SELECT
COUNT(1) AS `A1`
FROM `hooks` AS `Extent1`
 WHERE ((1 = `Extent1`.`hook_id`) AND ('NOTIFY_INTERNAL' = `Extent1`.`event`)) AND ((`Extent1`.`p0` = 6032) OR ((`Extent1`.`p0` IS  NULL) AND (6032 IS  NULL)))) AS `GroupBy1`;

SELECT
`GroupBy1`.`A1` AS `C1`
FROM (SELECT
COUNT(1) AS `A1`
FROM `notifications` AS `Extent1`
 WHERE (`Extent1`.`status` != 1) AND (`Extent1`.`employee` = 32)) AS `GroupBy1`;

SELECT
`GroupBy1`.`A1` AS `C1`
FROM (SELECT
COUNT(1) AS `A1`
FROM `hooks` AS `Extent1`
 WHERE ((1 = `Extent1`.`hook_id`) AND ('NOTIFY_INTERNAL' = `Extent1`.`event`)) AND ((`Extent1`.`p0` = 6032) OR ((`Extent1`.`p0` IS  NULL) AND (6032 IS  NULL)))) AS `GroupBy1`;

SELECT
`GroupBy1`.`A1` AS `C1`
FROM (SELECT
SUM(CASE WHEN (`Project1`.`C2` IS NOT NULL) THEN (`Project1`.`C1`)  ELSE (0) END) AS `A1`
FROM (SELECT
1 AS `X`) AS `SingleRowTable1` LEFT OUTER JOIN (SELECT
`Extent1`.`price` * (`Extent1`.`count`) AS `C1`, 
1 AS `C2`
FROM `works` AS `Extent1`
 WHERE `Extent1`.`repair` = 24903) AS `Project1` ON 1 = 1) AS `GroupBy1`;

SELECT
`GroupBy1`.`A1` AS `C1`
FROM (SELECT
SUM(CASE WHEN (`Project1`.`C2` IS NOT NULL) THEN (`Project1`.`C1`)  ELSE (0) END) AS `A1`
FROM (SELECT
1 AS `X`) AS `SingleRowTable1` LEFT OUTER JOIN (SELECT
(`Extent1`.`count`) * `Extent1`.`price` AS `C1`, 
1 AS `C2`
FROM `store_int_reserve` AS `Extent1`
 WHERE (`Extent1`.`repair_id` = 24903) AND ((2 = `Extent1`.`state`) OR (3 = `Extent1`.`state`))) AS `Project1` ON 1 = 1) AS `GroupBy1`;

SELECT
`Extent1`.`id`, `Extent1`.`item_id`, `Extent1`.`count`, `Extent1`.`created`, `Extent1`.`from_user`, `Extent1`.`to_user`, `Extent1`.`notes`, `Extent1`.`state`, `Extent1`.`repair_id`, 
`Extent1`.`price`, `Extent1`.`sn`, `Extent1`.`work_id`, `Extent1`.`warranty`, `Extent1`.`name`, `Extent1`.`r_lock`, `Extent2`.`id` AS `id1`, `Extent2`.`articul`, `Extent2`.`dealer`, 
`Extent2`.`is_realization`, `Extent2`.`dealer_lock`, `Extent2`.`name` AS `name1`, `Extent2`.`state` AS `state1`, `Extent2`.`category`, `Extent2`.`store`, `Extent2`.`created` AS `created1`, 
`Extent2`.`count` AS `count1`, `Extent2`.`reserved`, `Extent2`.`box`, `Extent2`.`box_name`, `Extent2`.`price_option`, `Extent2`.`currency_rate`, `Extent2`.`in_price`, `Extent2`.`price` AS 
`price1`, `Extent2`.`price2`, `Extent2`.`price3`, `Extent2`.`price4`, `Extent2`.`price5`, `Extent2`.`document`, `Extent2`.`shop_title`, `Extent2`.`shop_description`, `Extent2`.`SN` AS `sn1`, 
`Extent2`.`PN`, `Extent2`.`description`, `Extent2`.`shop_enable`, `Extent2`.`int_barcode`, `Extent2`.`ext_barcode`, `Extent2`.`in_summ`, `Extent2`.`notes` AS `notes1`, `Extent2`.`img1`, 
`Extent2`.`img2`, `Extent2`.`img3`, `Extent2`.`img4`, `Extent2`.`img5`, `Extent2`.`minimum_in_stock`, `Extent2`.`sold`, `Extent2`.`return_percent`, `Extent2`.`warranty` AS `warranty1`, 
`Extent2`.`warranty_dealer`, `Extent2`.`in_count`, `Extent2`.`not_for_sale`, `Extent2`.`part_request`, `Extent2`.`st_state`, `Extent2`.`st_notes`, `Extent2`.`units`, 
`Extent2`.`ge_highlight`, `Extent2`.`Hidden`, `Extent2`.`updated`
FROM `store_int_reserve` AS `Extent1` INNER JOIN `store_items` AS `Extent2` ON `Extent1`.`item_id` = `Extent2`.`id`
 WHERE (`Extent1`.`repair_id` = 24903) AND ((2 = `Extent1`.`state`) OR (3 = `Extent1`.`state`));

SELECT
`Extent1`.`id`, `Extent1`.`client`, `Extent1`.`type`, `Extent1`.`maker`, `Extent1`.`model`, `Extent1`.`serial_number`, `Extent1`.`company`, `Extent1`.`office`, `Extent1`.`start_office`, 
`Extent1`.`manager`, `Extent1`.`current_manager`, `Extent1`.`master`, `Extent1`.`diagnostic_result`, `Extent1`.`in_date`, `Extent1`.`out_date`, `Extent1`.`state`, `Extent1`.`user_lock`, 
`Extent1`.`lock_datetime`, `Extent1`.`express_repair`, `Extent1`.`is_warranty`, `Extent1`.`is_repeat`, `Extent1`.`can_format`, `Extent1`.`print_check`, `Extent1`.`box`, 
`Extent1`.`warranty_label`, `Extent1`.`ext_notes`, `Extent1`.`is_prepaid`, `Extent1`.`prepaid_type`, `Extent1`.`prepaid_summ`, `Extent1`.`prepaid_order`, `Extent1`.`is_pre_agreed`, 
`Extent1`.`pre_agreed_amount`, `Extent1`.`repair_cost`, `Extent1`.`fault`, `Extent1`.`complect`, `Extent1`.`look`, `Extent1`.`thirs_party_sc`, `Extent1`.`last_save`, 
`Extent1`.`last_status_changed`, `Extent1`.`warranty_days`, `Extent1`.`barcode`, `Extent1`.`new_state`, `Extent1`.`is_card_payment`, `Extent1`.`informed_status`, `Extent1`.`parts_cost`, 
`Extent1`.`is_debt`, `Extent1`.`image_ids`, `Extent1`.`color`, `Extent1`.`order_moving`, `Extent1`.`payment_system`, `Extent1`.`early`, `Extent1`.`real_repair_cost`, `Extent1`.`issued_msg`, 
`Extent1`.`reject_reason`, `Extent1`.`ext_early`, `Extent1`.`sms_inform`, `Extent1`.`invoice`, `Extent1`.`cartridge`, `Extent1`.`termsControl`, `Extent1`.`vendor_id`, 
`Extent1`.`quick_repair`, `Extent1`.`Hidden`, `Extent1`.`Title`
FROM `workshop` AS `Extent1`
 WHERE `Extent1`.`id` = 24903;

SELECT UTC_TIMESTAMP() FROM DUAL;

SELECT
`Extent1`.`id`, 
`Extent1`.`name`, 
`Extent1`.`places`, 
`Extent1`.`store_id`, 
`Extent1`.`non_items`, 
`Extent1`.`color`
FROM `boxes` AS `Extent1`
 WHERE `Extent1`.`id` = 1036;

BEGIN;

UPDATE `workshop` SET `out_date`='2022-03-23 13:07:40', `state`=8, `lock_datetime`='2022-03-23 13:07:27', `box`=NULL, `last_status_changed`='2022-03-23 13:07:40' WHERE `id` = 24903;

INSERT INTO `workshop_issued`(`repair_id`, `employee_id`, `created_at`) VALUES (%1, %2, UTC_TIMESTAMP());

SELECT
`id`
FROM `workshop_issued`
 WHERE  row_count() > 0 AND `id`=last_insert_id();

COMMIT;

BEGIN;

INSERT INTO `logs`(
`group`, `type`, `arh`, `user`, `created`, `values`, `values_after`, `office`, `client`, `repair`, `item`, `document`, `cash_order`, `part_request`, `notes`) VALUES (
NULL, 3, 0, 32, '2022-03-23 13:07:40.165239', NULL, NULL, 1, NULL, 24903, NULL, NULL, NULL, NULL, 'Заказаз-наряд: 024903, изъят из ячейки RPR/C2');

SELECT
`id`
FROM `logs`
 WHERE  row_count() > 0 AND `id`=last_insert_id();

INSERT INTO `logs`(
`group`, `type`, `arh`, `user`, `created`, `values`, `values_after`, `office`, `client`, `repair`, `item`, `document`, `cash_order`, `part_request`, `notes`) VALUES (
NULL, 3, 0, 32, '2022-03-23 13:07:40.202322', NULL, NULL, 1, NULL, 24903, NULL, NULL, NULL, NULL, 'Заказаз-наряд: 024903, выдан клиенту. Сумма: 100,00 грн., оплата: Наличные');

SELECT
`id`
FROM `logs`
 WHERE  row_count() > 0 AND `id`=last_insert_id();

COMMIT;

SELECT UTC_TIMESTAMP() FROM DUAL;

BEGIN;

INSERT INTO `repair_status_logs`(`created_at`, `repair_id`, `status_id`, `user_id`, `manager_id`, `master_id`) VALUES (UTC_TIMESTAMP(), %1, 6, %2, %3, 32);").arg((R)).arg((U)).arg((M))

SELECT
`id`
FROM `repair_status_logs`
 WHERE  row_count() > 0 AND `id`=last_insert_id();

COMMIT;

SELECT
`GroupBy1`.`A1` AS `C1`
FROM (SELECT
SUM(CASE WHEN (`Project1`.`C2` IS NOT NULL) THEN (`Project1`.`C1`)  ELSE (0) END) AS `A1`
FROM (SELECT
1 AS `X`) AS `SingleRowTable1` LEFT OUTER JOIN (SELECT
`Extent1`.`price` * (`Extent1`.`count`) AS `C1`, 
1 AS `C2`
FROM `works` AS `Extent1`
 WHERE `Extent1`.`repair` = 24903) AS `Project1` ON 1 = 1) AS `GroupBy1`;

SELECT
`GroupBy1`.`A1` AS `C1`
FROM (SELECT
SUM(CASE WHEN (`Project1`.`C2` IS NOT NULL) THEN (`Project1`.`C1`)  ELSE (0) END) AS `A1`
FROM (SELECT
1 AS `X`) AS `SingleRowTable1` LEFT OUTER JOIN (SELECT
(`Extent1`.`count`) * `Extent1`.`price` AS `C1`, 
1 AS `C2`
FROM `store_int_reserve` AS `Extent1`
 WHERE (`Extent1`.`repair_id` = 24903) AND ((2 = `Extent1`.`state`) OR (3 = `Extent1`.`state`))) AS `Project1` ON 1 = 1) AS `GroupBy1`;

SELECT
`GroupBy1`.`A1` AS `C1`
FROM (SELECT
SUM(CASE WHEN (`Project1`.`C1` IS NOT NULL) THEN (`Project1`.`summa`)  ELSE (0) END) AS `A1`
FROM (SELECT
1 AS `X`) AS `SingleRowTable1` LEFT OUTER JOIN (SELECT
`Extent1`.`summa`, 
1 AS `C1`
FROM `cash_orders` AS `Extent1`
 WHERE `Extent1`.`repair` = 24903) AS `Project1` ON 1 = 1) AS `GroupBy1`;

SELECT
`Project2`.`id`, `Project2`.`creator`, `Project2`.`name`, `Project2`.`surname`, `Project2`.`patronymic`, `Project2`.`agent_phone_mask`, `Project2`.`agent2_phone_mask`, `Project2`.`address`, 
`Project2`.`post_index`, `Project2`.`passport_num`, `Project2`.`passport_date`, `Project2`.`passport_organ`, `Project2`.`state`, `Project2`.`type`, `Project2`.`birthday`, 
`Project2`.`memorial`, `Project2`.`notes`, `Project2`.`is_regular`, `Project2`.`is_dealer`, `Project2`.`balance_enable`, `Project2`.`prefer_cashless`, `Project2`.`take_long`, 
`Project2`.`ignore_calls`, `Project2`.`is_bad`, `Project2`.`is_realizator`, `Project2`.`is_agent`, `Project2`.`INN`, `Project2`.`KPP`, `Project2`.`OGRN`, `Project2`.`web_password`, 
`Project2`.`ur_name`, `Project2`.`email`, `Project2`.`icq`, `Project2`.`skype`, `Project2`.`viber`, `Project2`.`telegram`, `Project2`.`site`, `Project2`.`whatsapp`, `Project2`.`agent_name`, 
`Project2`.`agent_surname`, `Project2`.`agent_patronymic`, `Project2`.`agent_phone`, `Project2`.`agent2_name`, `Project2`.`agent2_surname`, `Project2`.`agent2_patronymic`, 
`Project2`.`agent2_phone`, `Project2`.`created`, `Project2`.`balance`, `Project2`.`price_col`, `Project2`.`agent_phone_clean`, `Project2`.`agent2_phone_clean`, `Project2`.`repairs`, 
`Project2`.`token`, `Project2`.`purchases`, `Project2`.`visit_source`, `Project2`.`photo_id`, `Project2`.`C1`, `Project2`.`id1`, `Project2`.`phone`, `Project2`.`phone_clean`, 
`Project2`.`mask`, `Project2`.`customer`, `Project2`.`type1`, `Project2`.`note`
FROM (SELECT
`Limit1`.`id`, `Limit1`.`creator`, `Limit1`.`name`, `Limit1`.`surname`, `Limit1`.`patronymic`, `Limit1`.`agent_phone_mask`, `Limit1`.`agent2_phone_mask`, `Limit1`.`address`, 
`Limit1`.`post_index`, `Limit1`.`passport_num`, `Limit1`.`passport_date`, `Limit1`.`passport_organ`, `Limit1`.`state`, `Limit1`.`type`, `Limit1`.`birthday`, `Limit1`.`memorial`, 
`Limit1`.`notes`, `Limit1`.`is_regular`, `Limit1`.`is_dealer`, `Limit1`.`balance_enable`, `Limit1`.`prefer_cashless`, `Limit1`.`take_long`, `Limit1`.`ignore_calls`, `Limit1`.`is_bad`, 
`Limit1`.`is_realizator`, `Limit1`.`is_agent`, `Limit1`.`visit_source`, `Limit1`.`photo_id`, `Limit1`.`INN`, `Limit1`.`KPP`, `Limit1`.`OGRN`, `Limit1`.`web_password`, `Limit1`.`ur_name`, 
`Limit1`.`email`, `Limit1`.`icq`, `Limit1`.`skype`, `Limit1`.`viber`, `Limit1`.`telegram`, `Limit1`.`site`, `Limit1`.`whatsapp`, `Limit1`.`agent_name`, `Limit1`.`agent_surname`, 
`Limit1`.`agent_patronymic`, `Limit1`.`agent_phone`, `Limit1`.`agent_phone_clean`, `Limit1`.`agent2_name`, `Limit1`.`agent2_surname`, `Limit1`.`agent2_patronymic`, `Limit1`.`agent2_phone`, 
`Limit1`.`agent2_phone_clean`, `Limit1`.`created`, `Limit1`.`balance`, `Limit1`.`price_col`, `Limit1`.`repairs`, `Limit1`.`purchases`, `Limit1`.`token`, `Extent2`.`id` AS `id1`, 
`Extent2`.`phone`, `Extent2`.`phone_clean`, `Extent2`.`mask`, `Extent2`.`customer`, `Extent2`.`type` AS `type1`, `Extent2`.`note`, CASE WHEN (`Extent2`.`id` IS NOT NULL) THEN (1)  ELSE 
(NULL) END AS `C1`
FROM (SELECT
`Extent1`.`id`, `Extent1`.`creator`, `Extent1`.`name`, `Extent1`.`surname`, `Extent1`.`patronymic`, `Extent1`.`agent_phone_mask`, `Extent1`.`agent2_phone_mask`, `Extent1`.`address`, 
`Extent1`.`post_index`, `Extent1`.`passport_num`, `Extent1`.`passport_date`, `Extent1`.`passport_organ`, `Extent1`.`state`, `Extent1`.`type`, `Extent1`.`birthday`, `Extent1`.`memorial`, 
`Extent1`.`notes`, `Extent1`.`is_regular`, `Extent1`.`is_dealer`, `Extent1`.`balance_enable`, `Extent1`.`prefer_cashless`, `Extent1`.`take_long`, `Extent1`.`ignore_calls`, 
`Extent1`.`is_bad`, `Extent1`.`is_realizator`, `Extent1`.`is_agent`, `Extent1`.`visit_source`, `Extent1`.`photo_id`, `Extent1`.`INN`, `Extent1`.`KPP`, `Extent1`.`OGRN`, 
`Extent1`.`web_password`, `Extent1`.`ur_name`, `Extent1`.`email`, `Extent1`.`icq`, `Extent1`.`skype`, `Extent1`.`viber`, `Extent1`.`telegram`, `Extent1`.`site`, `Extent1`.`whatsapp`, 
`Extent1`.`agent_name`, `Extent1`.`agent_surname`, `Extent1`.`agent_patronymic`, `Extent1`.`agent_phone`, `Extent1`.`agent_phone_clean`, `Extent1`.`agent2_name`, `Extent1`.`agent2_surname`, 
`Extent1`.`agent2_patronymic`, `Extent1`.`agent2_phone`, `Extent1`.`agent2_phone_clean`, `Extent1`.`created`, `Extent1`.`balance`, `Extent1`.`price_col`, `Extent1`.`repairs`, 
`Extent1`.`purchases`, `Extent1`.`token`
FROM `clients` AS `Extent1`
 WHERE `Extent1`.`id` = 1285 LIMIT 1) AS `Limit1` LEFT OUTER JOIN `tel` AS `Extent2` ON `Limit1`.`id` = `Extent2`.`customer`) AS `Project2`
 ORDER BY 
`Project2`.`id` ASC, 
`Project2`.`C1` ASC;

SELECT UTC_TIMESTAMP() FROM DUAL;

BEGIN;

INSERT INTO `cash_orders`(
`created`, `type`, `summa`, `summa_str`, `invoice`, `client`, `to_user`, `user`, `company`, `office`, `notes`, `repair`, `document`, `img`, `payment_system`, `card_fee`, `is_backdate`, `card_info`, `customer_email`, `fdn`, `payment_item_sign`) VALUES (
'2022-03-23 13:07:40', 15, 100.0000, 'сто гривен 00 коп.', NULL, 1285, NULL, 32, 1, 1, 'Поступление денег в размере 100,00 грн. в счёт выполненого ремонта №024903', 24903, NULL, NULL, 0, 0, 0, NULL, '', NULL, NULL);

SELECT
`id`
FROM `cash_orders`
 WHERE  row_count() > 0 AND `id`=last_insert_id();

COMMIT;

SELECT
`Extent1`.`id`, `Extent1`.`client`, `Extent1`.`type`, `Extent1`.`maker`, `Extent1`.`model`, `Extent1`.`serial_number`, `Extent1`.`company`, `Extent1`.`office`, `Extent1`.`start_office`, 
`Extent1`.`manager`, `Extent1`.`current_manager`, `Extent1`.`master`, `Extent1`.`diagnostic_result`, `Extent1`.`in_date`, `Extent1`.`out_date`, `Extent1`.`state`, `Extent1`.`user_lock`, 
`Extent1`.`lock_datetime`, `Extent1`.`express_repair`, `Extent1`.`is_warranty`, `Extent1`.`is_repeat`, `Extent1`.`can_format`, `Extent1`.`print_check`, `Extent1`.`box`, 
`Extent1`.`warranty_label`, `Extent1`.`ext_notes`, `Extent1`.`is_prepaid`, `Extent1`.`prepaid_type`, `Extent1`.`prepaid_summ`, `Extent1`.`prepaid_order`, `Extent1`.`is_pre_agreed`, 
`Extent1`.`pre_agreed_amount`, `Extent1`.`repair_cost`, `Extent1`.`fault`, `Extent1`.`complect`, `Extent1`.`look`, `Extent1`.`thirs_party_sc`, `Extent1`.`last_save`, 
`Extent1`.`last_status_changed`, `Extent1`.`warranty_days`, `Extent1`.`barcode`, `Extent1`.`new_state`, `Extent1`.`is_card_payment`, `Extent1`.`informed_status`, `Extent1`.`parts_cost`, 
`Extent1`.`is_debt`, `Extent1`.`image_ids`, `Extent1`.`color`, `Extent1`.`order_moving`, `Extent1`.`payment_system`, `Extent1`.`early`, `Extent1`.`real_repair_cost`, `Extent1`.`issued_msg`, 
`Extent1`.`reject_reason`, `Extent1`.`ext_early`, `Extent1`.`sms_inform`, `Extent1`.`invoice`, `Extent1`.`cartridge`, `Extent1`.`termsControl`, `Extent1`.`vendor_id`, 
`Extent1`.`quick_repair`, `Extent1`.`Hidden`, `Extent1`.`Title`
FROM `workshop` AS `Extent1`
 WHERE `Extent1`.`id` = 24903 LIMIT 2;

SELECT
`GroupBy1`.`A1` AS `C1`
FROM (SELECT
COUNT(1) AS `A1`
FROM `hooks` AS `Extent1`
 WHERE ((1 = `Extent1`.`hook_id`) AND ('NOTIFY_INTERNAL' = `Extent1`.`event`)) AND ((`Extent1`.`p0` = 6032) OR ((`Extent1`.`p0` IS  NULL) AND (6032 IS  NULL)))) AS `GroupBy1`;

SELECT
`Project2`.`id`, `Project2`.`client`, `Project2`.`type`, `Project2`.`maker`, `Project2`.`model`, `Project2`.`serial_number`, `Project2`.`company`, `Project2`.`office`, 
`Project2`.`start_office`, `Project2`.`manager`, `Project2`.`current_manager`, `Project2`.`master`, `Project2`.`diagnostic_result`, `Project2`.`in_date`, `Project2`.`out_date`, 
`Project2`.`state`, `Project2`.`user_lock`, `Project2`.`lock_datetime`, `Project2`.`express_repair`, `Project2`.`is_warranty`, `Project2`.`is_repeat`, `Project2`.`can_format`, 
`Project2`.`print_check`, `Project2`.`box`, `Project2`.`warranty_label`, `Project2`.`ext_notes`, `Project2`.`is_prepaid`, `Project2`.`prepaid_type`, `Project2`.`prepaid_summ`, 
`Project2`.`prepaid_order`, `Project2`.`is_pre_agreed`, `Project2`.`pre_agreed_amount`, `Project2`.`repair_cost`, `Project2`.`fault`, `Project2`.`complect`, `Project2`.`look`, 
`Project2`.`thirs_party_sc`, `Project2`.`last_save`, `Project2`.`last_status_changed`, `Project2`.`warranty_days`, `Project2`.`barcode`, `Project2`.`new_state`, `Project2`.`is_card_payment`, 
`Project2`.`informed_status`, `Project2`.`parts_cost`, `Project2`.`is_debt`, `Project2`.`image_ids`, `Project2`.`color`, `Project2`.`order_moving`, `Project2`.`payment_system`, 
`Project2`.`early`, `Project2`.`real_repair_cost`, `Project2`.`issued_msg`, `Project2`.`reject_reason`, `Project2`.`ext_early`, `Project2`.`sms_inform`, `Project2`.`invoice`, 
`Project2`.`cartridge`, `Project2`.`termsControl`, `Project2`.`vendor_id`, `Project2`.`quick_repair`, `Project2`.`Hidden`, `Project2`.`Title`, `Project2`.`id1`, `Project2`.`sip_user_id`, 
`Project2`.`username`, `Project2`.`name`, `Project2`.`surname`, `Project2`.`patronymic`, `Project2`.`phone`, `Project2`.`phone2`, `Project2`.`phone_mask`, `Project2`.`phone2_mask`, 
`Project2`.`address`, `Project2`.`passport_num`, `Project2`.`passport_date`, `Project2`.`passport_organ`, `Project2`.`state1`, `Project2`.`created`, `Project2`.`office1`, 
`Project2`.`birthday`, `Project2`.`last_login`, `Project2`.`last_activity`, `Project2`.`email`, `Project2`.`sex`, `Project2`.`photo`, `Project2`.`salary_rate`, `Project2`.`pay_day`, 
`Project2`.`pay_day_off`, `Project2`.`pay_repair`, `Project2`.`pay_sale`, `Project2`.`pay_repair_q_sale`, `Project2`.`pay_cartridge_refill`, `Project2`.`color_label_ws`, 
`Project2`.`workspace_mode`, `Project2`.`preview_before_print`, `Project2`.`new_rep_doc_copies`, `Project2`.`auto_refresh_workspace`, `Project2`.`refresh_time`, `Project2`.`xls_c1`, 
`Project2`.`xls_c2`, `Project2`.`xls_c3`, `Project2`.`xls_c4`, `Project2`.`xls_c5`, `Project2`.`xls_c6`, `Project2`.`xls_c7`, `Project2`.`xls_c8`, `Project2`.`xls_c9`, `Project2`.`xls_c10`, 
`Project2`.`xls_c11`, `Project2`.`xls_c12`, `Project2`.`xls_c13`, `Project2`.`xls_c14`, `Project2`.`xls_c15`, `Project2`.`display_out`, `Project2`.`display_complete`, `Project2`.`is_bot`, 
`Project2`.`new_on_top`, `Project2`.`issued_color`, `Project2`.`fields_cfg`, `Project2`.`kkm_pass`, `Project2`.`prefer_regular`, `Project2`.`fontsize`, `Project2`.`rowheight`, 
`Project2`.`animation`, `Project2`.`pay_device_in`, `Project2`.`pay_device_out`, `Project2`.`pay_4_sale_in_repair`, `Project2`.`row_color`, `Project2`.`save_state_on_close`, 
`Project2`.`group_store_items`, `Project2`.`track_activity`, `Project2`.`inn`, `Project2`.`inform_comment`, `Project2`.`inform_status`, `Project2`.`kkt`, `Project2`.`pinpad`, 
`Project2`.`def_office`, `Project2`.`def_store`, `Project2`.`def_item_state`, `Project2`.`def_employee`, `Project2`.`def_status`, `Project2`.`def_ws_filter`, `Project2`.`card_on_call`, 
`Project2`.`ge_highlight_color`, `Project2`.`pay_repair_quick`, `Project2`.`advance_disable`, `Project2`.`salary_disable`, `Project2`.`notes`, `Project2`.`id2`, `Project2`.`creator`, 
`Project2`.`name1`, `Project2`.`surname1`, `Project2`.`patronymic1`, `Project2`.`agent_phone_mask`, `Project2`.`agent2_phone_mask`, `Project2`.`address1`, `Project2`.`post_index`, 
`Project2`.`passport_num1`, `Project2`.`passport_date1`, `Project2`.`passport_organ1`, `Project2`.`state2`, `Project2`.`type1`, `Project2`.`birthday1`, `Project2`.`memorial`, 
`Project2`.`notes1`, `Project2`.`is_regular`, `Project2`.`is_dealer`, `Project2`.`balance_enable`, `Project2`.`prefer_cashless`, `Project2`.`take_long`, `Project2`.`ignore_calls`, 
`Project2`.`is_bad`, `Project2`.`is_realizator`, `Project2`.`is_agent`, `Project2`.`inn1`, `Project2`.`KPP`, `Project2`.`OGRN`, `Project2`.`web_password`, `Project2`.`ur_name`, 
`Project2`.`email1`, `Project2`.`icq`, `Project2`.`skype`, `Project2`.`viber`, `Project2`.`telegram`, `Project2`.`site`, `Project2`.`whatsapp`, `Project2`.`agent_name`, 
`Project2`.`agent_surname`, `Project2`.`agent_patronymic`, `Project2`.`agent_phone`, `Project2`.`agent2_name`, `Project2`.`agent2_surname`, `Project2`.`agent2_patronymic`, 
`Project2`.`agent2_phone`, `Project2`.`created1`, `Project2`.`balance`, `Project2`.`price_col`, `Project2`.`agent_phone_clean`, `Project2`.`agent2_phone_clean`, `Project2`.`repairs`, 
`Project2`.`token`, `Project2`.`purchases`, `Project2`.`visit_source`, `Project2`.`photo_id`, `Project2`.`id3`, `Project2`.`num`, `Project2`.`created2`, `Project2`.`user`, 
`Project2`.`seller`, `Project2`.`customer`, `Project2`.`paid`, `Project2`.`tax`, `Project2`.`summ`, `Project2`.`total`, `Project2`.`state3`, `Project2`.`office2`, `Project2`.`type2`, 
`Project2`.`notes2`, `Project2`.`id4`, `Project2`.`refill`, `Project2`.`chip`, `Project2`.`opc_drum`, `Project2`.`c_blade`, `Project2`.`card_id`, `Project2`.`id5`, `Project2`.`name2`, 
`Project2`.`maker1`, `Project2`.`full_weight`, `Project2`.`toner_weight`, `Project2`.`resource`, `Project2`.`created3`, `Project2`.`user1`, `Project2`.`notes3`, `Project2`.`photo1`, 
`Project2`.`color1`, `Project2`.`archive`, `Project2`.`C1`, `Project2`.`id6`, `Project2`.`field_id`, `Project2`.`repair_id`, `Project2`.`value`, `Project2`.`item_id`, `Project2`.`id7`, 
`Project2`.`name3`, `Project2`.`type3`, `Project2`.`def_values`, `Project2`.`required`, `Project2`.`printable`, `Project2`.`archive1`, `Project2`.`_f`, `Project2`.`updated_at`, 
`Project2`.`devices`, `Project2`.`categories`
FROM (SELECT
`Limit1`.`id`, `Limit1`.`Hidden`, `Limit1`.`Title`, `Limit1`.`client`, `Limit1`.`type`, `Limit1`.`maker`, `Limit1`.`model`, `Limit1`.`serial_number`, `Limit1`.`company`, `Limit1`.`office`, 
`Limit1`.`start_office`, `Limit1`.`manager`, `Limit1`.`current_manager`, `Limit1`.`master`, `Limit1`.`diagnostic_result`, `Limit1`.`in_date`, `Limit1`.`out_date`, `Limit1`.`state`, 
`Limit1`.`new_state`, `Limit1`.`user_lock`, `Limit1`.`lock_datetime`, `Limit1`.`express_repair`, `Limit1`.`quick_repair`, `Limit1`.`is_warranty`, `Limit1`.`is_repeat`, 
`Limit1`.`payment_system`, `Limit1`.`is_card_payment`, `Limit1`.`can_format`, `Limit1`.`print_check`, `Limit1`.`box`, `Limit1`.`warranty_label`, `Limit1`.`ext_notes`, `Limit1`.`is_prepaid`, 
`Limit1`.`prepaid_type`, `Limit1`.`prepaid_summ`, `Limit1`.`prepaid_order`, `Limit1`.`is_pre_agreed`, `Limit1`.`is_debt`, `Limit1`.`pre_agreed_amount`, `Limit1`.`repair_cost`, 
`Limit1`.`real_repair_cost`, `Limit1`.`parts_cost`, `Limit1`.`fault`, `Limit1`.`complect`, `Limit1`.`look`, `Limit1`.`thirs_party_sc`, `Limit1`.`last_save`, `Limit1`.`last_status_changed`, 
`Limit1`.`warranty_days`, `Limit1`.`barcode`, `Limit1`.`reject_reason`, `Limit1`.`informed_status`, `Limit1`.`image_ids`, `Limit1`.`color`, `Limit1`.`order_moving`, `Limit1`.`early`, 
`Limit1`.`ext_early`, `Limit1`.`issued_msg`, `Limit1`.`sms_inform`, `Limit1`.`invoice`, `Limit1`.`cartridge`, `Limit1`.`vendor_id`, `Limit1`.`termsControl`, `Limit1`.`id1`, 
`Limit1`.`sip_user_id`, `Limit1`.`username`, `Limit1`.`name`, `Limit1`.`surname`, `Limit1`.`patronymic`, `Limit1`.`phone`, `Limit1`.`phone2`, `Limit1`.`phone_mask`, `Limit1`.`phone2_mask`, 
`Limit1`.`address`, `Limit1`.`passport_num`, `Limit1`.`passport_date`, `Limit1`.`passport_organ`, `Limit1`.`state1`, `Limit1`.`created`, `Limit1`.`office1`, `Limit1`.`birthday`, 
`Limit1`.`def_office`, `Limit1`.`def_store`, `Limit1`.`def_item_state`, `Limit1`.`def_employee`, `Limit1`.`def_status`, `Limit1`.`def_ws_filter`, `Limit1`.`last_login`, 
`Limit1`.`last_activity`, `Limit1`.`email`, `Limit1`.`sex`, `Limit1`.`photo`, `Limit1`.`salary_rate`, `Limit1`.`pay_day`, `Limit1`.`pay_day_off`, `Limit1`.`pay_repair`, 
`Limit1`.`pay_repair_quick`, `Limit1`.`pay_sale`, `Limit1`.`pay_repair_q_sale`, `Limit1`.`pay_cartridge_refill`, `Limit1`.`pay_device_in`, `Limit1`.`pay_device_out`, 
`Limit1`.`pay_4_sale_in_repair`, `Limit1`.`row_color`, `Limit1`.`ge_highlight_color`, `Limit1`.`color_label_ws`, `Limit1`.`workspace_mode`, `Limit1`.`preview_before_print`, 
`Limit1`.`new_rep_doc_copies`, `Limit1`.`auto_refresh_workspace`, `Limit1`.`refresh_time`, `Limit1`.`xls_c1`, `Limit1`.`xls_c2`, `Limit1`.`xls_c3`, `Limit1`.`xls_c4`, `Limit1`.`xls_c5`, 
`Limit1`.`xls_c6`, `Limit1`.`xls_c7`, `Limit1`.`xls_c8`, `Limit1`.`xls_c9`, `Limit1`.`xls_c10`, `Limit1`.`xls_c11`, `Limit1`.`xls_c12`, `Limit1`.`xls_c13`, `Limit1`.`xls_c14`, 
`Limit1`.`xls_c15`, `Limit1`.`display_out`, `Limit1`.`display_complete`, `Limit1`.`is_bot`, `Limit1`.`new_on_top`, `Limit1`.`issued_color`, `Limit1`.`kkm_pass`, `Limit1`.`prefer_regular`, 
`Limit1`.`fontsize`, `Limit1`.`rowheight`, `Limit1`.`animation`, `Limit1`.`fields_cfg`, `Limit1`.`save_state_on_close`, `Limit1`.`group_store_items`, `Limit1`.`track_activity`, 
`Limit1`.`card_on_call`, `Limit1`.`inn`, `Limit1`.`inform_comment`, `Limit1`.`inform_status`, `Limit1`.`kkt`, `Limit1`.`pinpad`, `Limit1`.`advance_disable`, `Limit1`.`salary_disable`, 
`Limit1`.`notes`, `Limit1`.`id2`, `Limit1`.`creator`, `Limit1`.`name1`, `Limit1`.`surname1`, `Limit1`.`patronymic1`, `Limit1`.`agent_phone_mask`, `Limit1`.`agent2_phone_mask`, 
`Limit1`.`address1`, `Limit1`.`post_index`, `Limit1`.`passport_num1`, `Limit1`.`passport_date1`, `Limit1`.`passport_organ1`, `Limit1`.`state2`, `Limit1`.`type1`, `Limit1`.`birthday1`, 
`Limit1`.`memorial`, `Limit1`.`notes1`, `Limit1`.`is_regular`, `Limit1`.`is_dealer`, `Limit1`.`balance_enable`, `Limit1`.`prefer_cashless`, `Limit1`.`take_long`, `Limit1`.`ignore_calls`, 
`Limit1`.`is_bad`, `Limit1`.`is_realizator`, `Limit1`.`is_agent`, `Limit1`.`visit_source`, `Limit1`.`photo_id`, `Limit1`.`inn1`, `Limit1`.`KPP`, `Limit1`.`OGRN`, `Limit1`.`web_password`, 
`Limit1`.`ur_name`, `Limit1`.`email1`, `Limit1`.`icq`, `Limit1`.`skype`, `Limit1`.`viber`, `Limit1`.`telegram`, `Limit1`.`site`, `Limit1`.`whatsapp`, `Limit1`.`agent_name`, 
`Limit1`.`agent_surname`, `Limit1`.`agent_patronymic`, `Limit1`.`agent_phone`, `Limit1`.`agent_phone_clean`, `Limit1`.`agent2_name`, `Limit1`.`agent2_surname`, `Limit1`.`agent2_patronymic`, 
`Limit1`.`agent2_phone`, `Limit1`.`agent2_phone_clean`, `Limit1`.`created1`, `Limit1`.`balance`, `Limit1`.`price_col`, `Limit1`.`repairs`, `Limit1`.`purchases`, `Limit1`.`token`, 
`Limit1`.`id3`, `Limit1`.`num`, `Limit1`.`created2`, `Limit1`.`user`, `Limit1`.`seller`, `Limit1`.`customer`, `Limit1`.`paid`, `Limit1`.`tax`, `Limit1`.`summ`, `Limit1`.`total`, 
`Limit1`.`state3`, `Limit1`.`office2`, `Limit1`.`type2`, `Limit1`.`notes2`, `Limit1`.`id4`, `Limit1`.`refill`, `Limit1`.`chip`, `Limit1`.`opc_drum`, `Limit1`.`c_blade`, `Limit1`.`card_id`, 
`Limit1`.`id5`, `Limit1`.`name2`, `Limit1`.`maker1`, `Limit1`.`full_weight`, `Limit1`.`toner_weight`, `Limit1`.`resource`, `Limit1`.`created3`, `Limit1`.`user1`, `Limit1`.`notes3`, 
`Limit1`.`photo1`, `Limit1`.`color1`, `Limit1`.`archive`, `Join6`.`id` AS `id6`, `Join6`.`field_id`, `Join6`.`repair_id`, `Join6`.`item_id`, `Join6`.`value`, `Join6`.`ID1` AS `id7`, 
`Join6`.`_f`, `Join6`.`name` AS `name3`, `Join6`.`type` AS `type3`, `Join6`.`def_values`, `Join6`.`required`, `Join6`.`printable`, `Join6`.`devices`, `Join6`.`categories`, 
`Join6`.`updated_at`, `Join6`.`archive` AS `archive1`, CASE WHEN (`Join6`.`id` IS NOT NULL) THEN (1)  ELSE (NULL) END AS `C1`
FROM (SELECT
`Extent1`.`id`, `Extent1`.`Hidden`, `Extent1`.`Title`, `Extent1`.`client`, `Extent1`.`type`, `Extent1`.`maker`, `Extent1`.`model`, `Extent1`.`serial_number`, `Extent1`.`company`, 
`Extent1`.`office`, `Extent1`.`start_office`, `Extent1`.`manager`, `Extent1`.`current_manager`, `Extent1`.`master`, `Extent1`.`diagnostic_result`, `Extent1`.`in_date`, `Extent1`.`out_date`, 
`Extent1`.`state`, `Extent1`.`new_state`, `Extent1`.`user_lock`, `Extent1`.`lock_datetime`, `Extent1`.`express_repair`, `Extent1`.`quick_repair`, `Extent1`.`is_warranty`, 
`Extent1`.`is_repeat`, `Extent1`.`payment_system`, `Extent1`.`is_card_payment`, `Extent1`.`can_format`, `Extent1`.`print_check`, `Extent1`.`box`, `Extent1`.`warranty_label`, 
`Extent1`.`ext_notes`, `Extent1`.`is_prepaid`, `Extent1`.`prepaid_type`, `Extent1`.`prepaid_summ`, `Extent1`.`prepaid_order`, `Extent1`.`is_pre_agreed`, `Extent1`.`is_debt`, 
`Extent1`.`pre_agreed_amount`, `Extent1`.`repair_cost`, `Extent1`.`real_repair_cost`, `Extent1`.`parts_cost`, `Extent1`.`fault`, `Extent1`.`complect`, `Extent1`.`look`, 
`Extent1`.`thirs_party_sc`, `Extent1`.`last_save`, `Extent1`.`last_status_changed`, `Extent1`.`warranty_days`, `Extent1`.`barcode`, `Extent1`.`reject_reason`, `Extent1`.`informed_status`, 
`Extent1`.`image_ids`, `Extent1`.`color`, `Extent1`.`order_moving`, `Extent1`.`early`, `Extent1`.`ext_early`, `Extent1`.`issued_msg`, `Extent1`.`sms_inform`, `Extent1`.`invoice`, 
`Extent1`.`cartridge`, `Extent1`.`vendor_id`, `Extent1`.`termsControl`, `Extent2`.`id` AS `id1`, `Extent2`.`sip_user_id`, `Extent2`.`username`, `Extent2`.`name`, `Extent2`.`surname`, 
`Extent2`.`patronymic`, `Extent2`.`phone`, `Extent2`.`phone2`, `Extent2`.`phone_mask`, `Extent2`.`phone2_mask`, `Extent2`.`address`, `Extent2`.`passport_num`, `Extent2`.`passport_date`, 
`Extent2`.`passport_organ`, `Extent2`.`state` AS `state1`, `Extent2`.`created`, `Extent2`.`office` AS `office1`, `Extent2`.`birthday`, `Extent2`.`def_office`, `Extent2`.`def_store`, 
`Extent2`.`def_item_state`, `Extent2`.`def_employee`, `Extent2`.`def_status`, `Extent2`.`def_ws_filter`, `Extent2`.`last_login`, `Extent2`.`last_activity`, `Extent2`.`email`, 
`Extent2`.`sex`, `Extent2`.`photo`, `Extent2`.`salary_rate`, `Extent2`.`pay_day`, `Extent2`.`pay_day_off`, `Extent2`.`pay_repair`, `Extent2`.`pay_repair_quick`, `Extent2`.`pay_sale`, 
`Extent2`.`pay_repair_q_sale`, `Extent2`.`pay_cartridge_refill`, `Extent2`.`pay_device_in`, `Extent2`.`pay_device_out`, `Extent2`.`pay_4_sale_in_repair`, `Extent2`.`row_color`, 
`Extent2`.`ge_highlight_color`, `Extent2`.`color_label_ws`, `Extent2`.`workspace_mode`, `Extent2`.`preview_before_print`, `Extent2`.`new_rep_doc_copies`, `Extent2`.`auto_refresh_workspace`, 
`Extent2`.`refresh_time`, `Extent2`.`xls_c1`, `Extent2`.`xls_c2`, `Extent2`.`xls_c3`, `Extent2`.`xls_c4`, `Extent2`.`xls_c5`, `Extent2`.`xls_c6`, `Extent2`.`xls_c7`, `Extent2`.`xls_c8`, 
`Extent2`.`xls_c9`, `Extent2`.`xls_c10`, `Extent2`.`xls_c11`, `Extent2`.`xls_c12`, `Extent2`.`xls_c13`, `Extent2`.`xls_c14`, `Extent2`.`xls_c15`, `Extent2`.`display_out`, 
`Extent2`.`display_complete`, `Extent2`.`is_bot`, `Extent2`.`new_on_top`, `Extent2`.`issued_color`, `Extent2`.`kkm_pass`, `Extent2`.`prefer_regular`, `Extent2`.`fontsize`, 
`Extent2`.`rowheight`, `Extent2`.`animation`, `Extent2`.`fields_cfg`, `Extent2`.`save_state_on_close`, `Extent2`.`group_store_items`, `Extent2`.`track_activity`, `Extent2`.`card_on_call`, 
`Extent2`.`inn`, `Extent2`.`inform_comment`, `Extent2`.`inform_status`, `Extent2`.`kkt`, `Extent2`.`pinpad`, `Extent2`.`advance_disable`, `Extent2`.`salary_disable`, `Extent2`.`notes`, 
`Extent3`.`id` AS `id2`, `Extent3`.`creator`, `Extent3`.`name` AS `name1`, `Extent3`.`surname` AS `surname1`, `Extent3`.`patronymic` AS `patronymic1`, `Extent3`.`agent_phone_mask`, 
`Extent3`.`agent2_phone_mask`, `Extent3`.`address` AS `address1`, `Extent3`.`post_index`, `Extent3`.`passport_num` AS `passport_num1`, `Extent3`.`passport_date` AS `passport_date1`, 
`Extent3`.`passport_organ` AS `passport_organ1`, `Extent3`.`state` AS `state2`, `Extent3`.`type` AS `type1`, `Extent3`.`birthday` AS `birthday1`, `Extent3`.`memorial`, `Extent3`.`notes` AS 
`notes1`, `Extent3`.`is_regular`, `Extent3`.`is_dealer`, `Extent3`.`balance_enable`, `Extent3`.`prefer_cashless`, `Extent3`.`take_long`, `Extent3`.`ignore_calls`, `Extent3`.`is_bad`, 
`Extent3`.`is_realizator`, `Extent3`.`is_agent`, `Extent3`.`visit_source`, `Extent3`.`photo_id`, `Extent3`.`INN` AS `inn1`, `Extent3`.`KPP`, `Extent3`.`OGRN`, `Extent3`.`web_password`, 
`Extent3`.`ur_name`, `Extent3`.`email` AS `email1`, `Extent3`.`icq`, `Extent3`.`skype`, `Extent3`.`viber`, `Extent3`.`telegram`, `Extent3`.`site`, `Extent3`.`whatsapp`, 
`Extent3`.`agent_name`, `Extent3`.`agent_surname`, `Extent3`.`agent_patronymic`, `Extent3`.`agent_phone`, `Extent3`.`agent_phone_clean`, `Extent3`.`agent2_name`, `Extent3`.`agent2_surname`, 
`Extent3`.`agent2_patronymic`, `Extent3`.`agent2_phone`, `Extent3`.`agent2_phone_clean`, `Extent3`.`created` AS `created1`, `Extent3`.`balance`, `Extent3`.`price_col`, `Extent3`.`repairs`, 
`Extent3`.`purchases`, `Extent3`.`token`, `Extent4`.`id` AS `id3`, `Extent4`.`num`, `Extent4`.`created` AS `created2`, `Extent4`.`user`, `Extent4`.`seller`, `Extent4`.`customer`, 
`Extent4`.`paid`, `Extent4`.`tax`, `Extent4`.`summ`, `Extent4`.`total`, `Extent4`.`state` AS `state3`, `Extent4`.`office` AS `office2`, `Extent4`.`type` AS `type2`, `Extent4`.`notes` AS 
`notes2`, `Extent5`.`id` AS `id4`, `Extent5`.`refill`, `Extent5`.`chip`, `Extent5`.`opc_drum`, `Extent5`.`c_blade`, `Extent5`.`card_id`, `Extent6`.`id` AS `id5`, `Extent6`.`name` AS `name2`, 
`Extent6`.`maker` AS `maker1`, `Extent6`.`full_weight`, `Extent6`.`toner_weight`, `Extent6`.`resource`, `Extent6`.`created` AS `created3`, `Extent6`.`user` AS `user1`, `Extent6`.`notes` AS 
`notes3`, `Extent6`.`photo` AS `photo1`, `Extent6`.`color` AS `color1`, `Extent6`.`archive`
FROM `workshop` AS `Extent1` LEFT OUTER JOIN `users` AS `Extent2` ON `Extent1`.`user_lock` = `Extent2`.`id` INNER JOIN `clients` AS `Extent3` ON `Extent1`.`client` = `Extent3`.`id` LEFT OUTER JOIN `invoice` AS `Extent4` ON `Extent1`.`invoice` = `Extent4`.`id` LEFT OUTER JOIN `c_workshop` AS `Extent5` ON `Extent1`.`cartridge` = `Extent5`.`id` LEFT OUTER JOIN `cartridge_cards` AS `Extent6` ON `Extent5`.`card_id` = `Extent6`.`id`
 WHERE `Extent1`.`id` = 24903 LIMIT 2) AS `Limit1` LEFT OUTER JOIN (SELECT
`Extent7`.`id`, `Extent7`.`field_id`, `Extent7`.`repair_id`, `Extent7`.`item_id`, `Extent7`.`value`, `Extent8`.`id` AS `ID1`, `Extent8`.`_f`, `Extent8`.`name`, `Extent8`.`type`, `Extent8`.`def_values`, `Extent8`.`required`, `Extent8`.`printable`, `Extent8`.`devices`, `Extent8`.`categories`, `Extent8`.`updated_at`, `Extent8`.`archive`
FROM `field_values` AS `Extent7` INNER JOIN `fields` AS `Extent8` ON `Extent7`.`field_id` = `Extent8`.`id`) AS `Join6` ON `Limit1`.`id` = `Join6`.`repair_id`) AS `Project2`
 ORDER BY 
`Project2`.`id` ASC, `Project2`.`id1` ASC, `Project2`.`id2` ASC, `Project2`.`id3` ASC, `Project2`.`id4` ASC, `Project2`.`id5` ASC, `Project2`.`C1` ASC;

SELECT
`GroupBy1`.`A1` AS `C1`
FROM (SELECT
COUNT(1) AS `A1`
FROM `notifications` AS `Extent1`
 WHERE (`Extent1`.`status` != 1) AND (`Extent1`.`employee` = 32)) AS `GroupBy1`;

SELECT
`GroupBy1`.`A1` AS `C1`
FROM (SELECT
COUNT(1) AS `A1`
FROM `hooks` AS `Extent1`
 WHERE ((1 = `Extent1`.`hook_id`) AND ('NOTIFY_INTERNAL' = `Extent1`.`event`)) AND ((`Extent1`.`p0` = 6032) OR ((`Extent1`.`p0` IS  NULL) AND (6032 IS  NULL)))) AS `GroupBy1`;

SELECT UTC_TIMESTAMP() FROM DUAL;

SELECT
`Extent1`.`id`, 
`Extent1`.`hook_id`, 
`Extent1`.`status`, 
`Extent1`.`created_at`, 
`Extent1`.`updated_at`, 
`Extent1`.`event`, 
`Extent1`.`p0`, 
`Extent1`.`prm`, 
`Extent1`.`task_id`
FROM `hooks` AS `Extent1`
 WHERE ((3 = `Extent1`.`hook_id`) AND ((`Extent1`.`event` IN ( 'NEW_ORDER','ORDER_CHANGED','NEW_CUSTOMER' )) AND (`Extent1`.`event` IS NOT NULL))) AND ((1 = `Extent1`.`status`) OR ((2 = `Extent1`.`status`) AND ('2022-03-23 12:57:41' > `Extent1`.`updated_at`)));

#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# выдача в долг; ремонт с добавленными запчастями.
SELECT UTC_TIMESTAMP() FROM DUAL;

SELECT
`Extent1`.`id`, 
`Extent1`.`hook_id`, 
`Extent1`.`status`, 
`Extent1`.`created_at`, 
`Extent1`.`updated_at`, 
`Extent1`.`event`, 
`Extent1`.`p0`, 
`Extent1`.`prm`, 
`Extent1`.`task_id`
FROM `hooks` AS `Extent1`
 WHERE ((3 = `Extent1`.`hook_id`) AND ((`Extent1`.`event` IN ( 'NEW_ORDER','ORDER_CHANGED','NEW_CUSTOMER' )) AND (`Extent1`.`event` IS NOT NULL))) AND ((1 = `Extent1`.`status`) OR ((2 = `Extent1`.`status`) AND ('2022-03-24 20:43:27' > `Extent1`.`updated_at`)));

SELECT
`GroupBy1`.`A1` AS `C1`
FROM (SELECT
COUNT(1) AS `A1`
FROM `hooks` AS `Extent1`
 WHERE ((1 = `Extent1`.`hook_id`) AND ('NOTIFY_INTERNAL' = `Extent1`.`event`)) AND ((`Extent1`.`p0` = 6032) OR ((`Extent1`.`p0` IS  NULL) AND (6032 IS  NULL)))) AS `GroupBy1`;

SELECT
`Extent1`.`id`, `Extent1`.`type`, `Extent1`.`state`, `Extent1`.`is_realization`, `Extent1`.`company`, `Extent1`.`store`, `Extent1`.`user`, `Extent1`.`total`, `Extent1`.`created`, 
`Extent1`.`office`, `Extent1`.`dealer`, `Extent1`.`currency_rate`, `Extent1`.`img1`, `Extent1`.`img2`, `Extent1`.`img3`, `Extent1`.`reason`, `Extent1`.`order_id`, `Extent1`.`price_option`, 
`Extent1`.`return_percent`, `Extent1`.`reserve_days`, `Extent1`.`master_id`, `Extent1`.`repair_id`, `Extent1`.`payment_system`, `Extent1`.`works_included`, `Extent1`.`invoice`, 
`Extent1`.`track`, `Extent1`.`notes`, `Extent1`.`d_store`, `Extent1`.`updated_at`, `Extent1`.`d_pay`
FROM `docs` AS `Extent1`
 WHERE ((6 = `Extent1`.`type`) AND (3 = `Extent1`.`state`)) AND ((DiffDays('2022-03-24 20:53:28.444405', `Extent1`.`created`)) > `Extent1`.`reserve_days`);

SELECT
`Extent1`.`sms_id`
FROM `notifications` AS `Extent1`
 WHERE ((3 = `Extent1`.`type`) AND (`Extent1`.`sms_id` IS NOT NULL)) AND (`Extent1`.`employee` = 32);

SELECT
`Extent1`.`id`, 
`Extent1`.`date`, 
`Extent1`.`modem`, 
`Extent1`.`callerid`, 
`Extent1`.`msg`, 
`Extent1`.`oktell`
FROM `in_sms` AS `Extent1`
 WHERE `Extent1`.`id` NOT  IN ( 6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,96,97,98,99,100,101,102,103,104,105,106,107,108,109,110,111,112,113,114,115,116,117,118,119,120,121,122,123,124,125,126,127,128,129,130,131,132,133,134,135,136,137,138,139,140,141,142,143,144,145,146,147,148,149,150,151,152,153,154,155,156,157,158,159,160,161,162,163,164,165,166,167,168,169,170,171,172,173,174,175,176,177,178,179,180,181,182,183,184,185,186,187,188,189,190,191,192,193,194,195,196,197,198,199,200,201,202,203,204,205,206,207,208,209,210,211,212,213,214,215,216,217,218,219,220,221,222,223,224,225,226,227,228,229,230,231,232,233,234,235,236,237,238,239,240,241,242,243,244,245,246,247,248,249,250,251,252,253,254,255,256,257,258,259,260,261,262,263,264,265,266,267,268,269,270,271,272,273,274,275,276,277,278,279,280,281,282,283,284,285,286,287,288,289,290,291,292,293,294,295,296,297,298,299,300,301,302,303,304,305,306,307,308,309,310,311,312,313,314,315,316,317,318,319,320,321,322,323,324,325,326,327,328,329,330,331,332,333,334,335,336,337,338,339,340,341,342,343,344,345,346,347,348,349,350,351,352,353,354,355,356,357,358,359,360,361,362,363,364,365,366,367,368,369,370,371,372,373,374,375,376,377,378,379,380,381,382,383,384,385,386,387,388,389,390,391,392,393,394,395,396,397,398,399,400,401,402,403,404,405,406,407,408,409,410,411,412,413,414,415,416,417,418,419,420,421,422,423,424,425,426,427,428,429,430,431,432,433,434,435,436,437,438,439,440,441,442,443,444,445,446,447,448,449,450,451,452,453,454,455,456,457,458,459,460,461,462,463,464,465,466,467,468,469,470,471,472,473,474,475,476,477,478,479,480,481,482,483,484,485,486,487,488,489,490,491,492,493,494,495,496,497,498,499,500,501,502,503,504,505,506,507,508,509,510,511,512,513,514,515,516,517,518,519,520,521,522,523,524,525,526,527,528,529,530,531,532,533,534,535,536,537,538,539,540,541,542,543,544,545,546,547,548,549,550,551,552,553,554,555,556,557,558,559,560,561,562,563,564,565,566,567,568,569,570,571,572,573,574,575,576,577,578,579,580,581,582,583,584,585,586,587,588,589,590,591,592,593,594,595,596,597,598,599,600,601,602,603,604,605,606,607,608,609,610,611,612,613,614,615,616,617,618,619,620,621,622,623,624,625,626,627,628,629,630,631,632,633,634,635,636,637,638,639,640,641,642,643,644,645,646,647,648,649,650,651,652,653,654,655,656,657,658,659,660,661,662,663,664,665,666,667,668,669,670,671,672,673,674,675,676,677,678,679,680,681,682,683,684,685,686,687,688,689,690,691,692,693,694,695,696,697,698,699,700,701,702,703,704,705,706,707,708,709,710,711,712,713,714,715,716,717,718,719,730,732,734,736,738,739,740,747,748,749,750,751,752,753,755,782,783,784,785,786,787,788,789,790,804,805,806,807,808,809,810,811,812,813,814,815,816,817,818,819,820,821,822,823,824,825,826,827,828,829,830,831,832,833,834,835,836,837,838,839,840,841,842,843,844,845,846,847,848,849,850,851,852,853,854,855,856,857,858,859,860,861,862,863,864,865,866,867,868,869,870,871,872,873,874,875,876,877,878,879,880,881,882,883,884,885,886,887,888,889,890,891,892,893,894,895,896,897,898,899,900,901,902,903,904,905,906,907,908,909,910,911,912,913,914,915,916,917,918,919,920,921,922,923,924,925,926,927,928,929,930,931,932,933,934,935,936,937,938,939,940,941,942,943,944,945,946,947,948,949,950,951,952,953,954,955,956,957,958,959,960,961,962,963,964,965,966,967,968,969,970,971,972,973,974,975,976,977,978,979,980,981,982,983,984,985,986,987,988,989,990,991,992,993,994,995,996,997,998,999,1000,1001,1002,1003,1004,1005,1006,1007,1008,1009,1010,1011,1012,1013,1014,1015,1016,1017,1018,1019,1020,1021,1022,1023,1024,1025,1026,1027,1028,1029,1030,1031,1032,1033,1034,1035,1036,1037,1038,1039,1040,1041,1042,1043,1044,1045,1046,1047,1048,1049,1050,1051,1052,1053,1054,1055,1056,1057,1058,1059,1060,1061,1062,1063,1064,1065,1066,1067,1068,1069,1070,1071,1072,1073,1074,1075,1076,1077,1078,1079,1080,1081,1082,1083,1084,1085,1086,1087,1088,1089,1090,1091,1092,1093,1094,1095,1096,1097,1098,1099,1100,1101,1102,1103,1104,1105,1106,1107,1108,1109,1110,1111,1112,1113,1114,1115,1116,1117,1118,1119,1120,1121,1122,1123,1124,1125,1126,1127,1128,1129,1130,1131,1132,1133,1134,1135,1136,1137,1138,1139,1140,1141,1142,1143,1144,1145,1146,1147,1148,1149,1150,1151,1152,1153,1154,1155,1156,1157,1158,1159,1160,1161,1162,1163,1164,1165,1166,1167,1168,1169,1170,1171,1172,1173,1174,1175,1176,1177,1178,1179,1180,1181,1182,1183,1184,1185,1186,1187,1188,1189,1190,1191,1192,1193,1194,1195,1196,1197,1198,1199,1200,1201,1202,1203,1204,1205,1206,1207,1208,1209,1210,1211,1212,1213,1214,1215,1216,1217,1218,1219,1220,1221,1222,1223,1224,1225,1226,1227,1228,1229,1230,1231,1232,1233,1234,1235,1236,1237,1238,1239,1240,1241,1242,1243,1244,1245,1246,1247,1248,1249,1250,1251,1252,1253,1254,1255,1256,1257,1258,1259,1260,1261,1262,1263,1264,1265,1266,1267,1268,1269,1270,1271,1272,1273,1274,1275,1276,1277,1278,1279,1280,1281,1282,1283,1284,1285,1286,1287,1288,1289,1290,1291,1292,1293,1294,1295,1296,1297,1298,1299,1300,1301,1302,1303,1304,1305,1306,1307,1308,1309,1310,1311,1312,1313,1314,1315,1316,1317,1318,1319,1320,1321,1322,1323,1324,1325,1326,1327,1328,1329,1330,1331,1332,1333,1334,1335,1336,1337,1338,1339,1340,1341,1342,1343,1344,1345,1346,1347,1348,1349,1350,1351,1352,1353,1354,1355,1356,1357,1358,1359,1360,1361,1362,1363,1364,1365,1366,1367,1368,1369,1370,1371,1372,1373,1374,1375,1376,1377,1378,1379,1380,1381,1382,1383,1384,1385,1386,1387,1388,1389,1390,1391,1392,1393,1394,1395,1396,1397,1398,1399,1400,1401,1402,1403,1404,1405,1406,1407,1408,1409,1410,1411,1412,1413,1414,1415,1416,1417,1418,1419,1420,1421,1422,1423,1424,1425,1426,1427,1428,1429,1430,1431,1432,1433,1434,1435,1436,1437,1438,1439,1440,1441,1442,1443,1444,1445,1446,1447,1448,1449,1450,1451,1452,1453,1454,1455,1456 );

SELECT
`Extent1`.`id`, `Extent1`.`hook_id`, `Extent1`.`status`, `Extent1`.`created_at`, `Extent1`.`updated_at`, `Extent1`.`event`, `Extent1`.`p0`, `Extent1`.`prm`, `Extent1`.`task_id`FROM `hooks` AS `Extent1`
 WHERE (1 = `Extent1`.`status`) AND ('CALLBACK' = `Extent1`.`event`);

SELECT UTC_TIMESTAMP() FROM DUAL;

SELECT
`GroupBy1`.`A1` AS `C1`
FROM (SELECT
COUNT(1) AS `A1`
FROM `notifications` AS `Extent1`
 WHERE (`Extent1`.`status` != 1) AND (`Extent1`.`employee` = 32)) AS `GroupBy1`;

SELECT
`GroupBy1`.`A1` AS `C1`
FROM (SELECT
COUNT(1) AS `A1`
FROM `notifications` AS `Extent1`
 WHERE (`Extent1`.`status` != 1) AND (`Extent1`.`employee` = 32)) AS `GroupBy1`;

SELECT
`GroupBy1`.`A1` AS `C1`
FROM (SELECT
COUNT(1) AS `A1`
FROM `notifications` AS `Extent1`
 WHERE (`Extent1`.`status` != 1) AND (`Extent1`.`employee` = 32)) AS `GroupBy1`;

SELECT
`GroupBy1`.`A1` AS `C1`
FROM (SELECT
COUNT(1) AS `A1`
FROM `notifications` AS `Extent1`
 WHERE (`Extent1`.`status` != 1) AND (`Extent1`.`employee` = 32)) AS `GroupBy1`;

SELECT
`GroupBy1`.`A1` AS `C1`
FROM (SELECT
COUNT(1) AS `A1`
FROM `hooks` AS `Extent1`
 WHERE ((1 = `Extent1`.`hook_id`) AND ('NOTIFY_INTERNAL' = `Extent1`.`event`)) AND ((`Extent1`.`p0` = 6032) OR ((`Extent1`.`p0` IS  NULL) AND (6032 IS  NULL)))) AS `GroupBy1`;

SELECT
`GroupBy1`.`A1` AS `C1`
FROM (SELECT
SUM(CASE WHEN (`Project1`.`C2` IS NOT NULL) THEN (`Project1`.`C1`)  ELSE (0) END) AS `A1`
FROM (SELECT
1 AS `X`) AS `SingleRowTable1` LEFT OUTER JOIN (SELECT
`Extent1`.`price` * (`Extent1`.`count`) AS `C1`, 
1 AS `C2`
FROM `works` AS `Extent1`
 WHERE `Extent1`.`repair` = 24882) AS `Project1` ON 1 = 1) AS `GroupBy1`;

SELECT
`GroupBy1`.`A1` AS `C1`
FROM (SELECT
SUM(CASE WHEN (`Project1`.`C2` IS NOT NULL) THEN (`Project1`.`C1`)  ELSE (0) END) AS `A1`
FROM (SELECT
1 AS `X`) AS `SingleRowTable1` LEFT OUTER JOIN (SELECT
(`Extent1`.`count`) * `Extent1`.`price` AS `C1`, 
1 AS `C2`
FROM `store_int_reserve` AS `Extent1`
 WHERE (`Extent1`.`repair_id` = 24882) AND ((2 = `Extent1`.`state`) OR (3 = `Extent1`.`state`))) AS `Project1` ON 1 = 1) AS `GroupBy1`;

SELECT
`Extent1`.`id`, `Extent1`.`item_id`, `Extent1`.`count`, `Extent1`.`created`, `Extent1`.`from_user`, `Extent1`.`to_user`, `Extent1`.`notes`, `Extent1`.`state`, `Extent1`.`repair_id`, 
`Extent1`.`price`, `Extent1`.`sn`, `Extent1`.`work_id`, `Extent1`.`warranty`, `Extent1`.`name`, `Extent1`.`r_lock`, `Extent2`.`id` AS `id1`, `Extent2`.`articul`, `Extent2`.`dealer`, 
`Extent2`.`is_realization`, `Extent2`.`dealer_lock`, `Extent2`.`name` AS `name1`, `Extent2`.`state` AS `state1`, `Extent2`.`category`, `Extent2`.`store`, `Extent2`.`created` AS `created1`, 
`Extent2`.`count` AS `count1`, `Extent2`.`reserved`, `Extent2`.`box`, `Extent2`.`box_name`, `Extent2`.`price_option`, `Extent2`.`currency_rate`, `Extent2`.`in_price`, `Extent2`.`price` AS 
`price1`, `Extent2`.`price2`, `Extent2`.`price3`, `Extent2`.`price4`, `Extent2`.`price5`, `Extent2`.`document`, `Extent2`.`shop_title`, `Extent2`.`shop_description`, `Extent2`.`SN` AS 
`sn1`, `Extent2`.`PN`, `Extent2`.`description`, `Extent2`.`shop_enable`, `Extent2`.`int_barcode`, `Extent2`.`ext_barcode`, `Extent2`.`in_summ`, `Extent2`.`notes` AS `notes1`, 
`Extent2`.`img1`, `Extent2`.`img2`, `Extent2`.`img3`, `Extent2`.`img4`, `Extent2`.`img5`, `Extent2`.`minimum_in_stock`, `Extent2`.`sold`, `Extent2`.`return_percent`, `Extent2`.`warranty` AS
 `warranty1`, `Extent2`.`warranty_dealer`, `Extent2`.`in_count`, `Extent2`.`not_for_sale`, `Extent2`.`part_request`, `Extent2`.`st_state`, `Extent2`.`st_notes`, `Extent2`.`units`, 
`Extent2`.`ge_highlight`, `Extent2`.`Hidden`, `Extent2`.`updated`
FROM `store_int_reserve` AS `Extent1` INNER JOIN `store_items` AS `Extent2` ON `Extent1`.`item_id` = `Extent2`.`id`
 WHERE (`Extent1`.`repair_id` = 24882) AND ((2 = `Extent1`.`state`) OR (3 = `Extent1`.`state`));

SELECT
`Extent1`.`id`, `Extent1`.`articul`, `Extent1`.`dealer`, `Extent1`.`is_realization`, `Extent1`.`dealer_lock`, `Extent1`.`name`, `Extent1`.`state`, `Extent1`.`category`, `Extent1`.`store`, 
`Extent1`.`created`, `Extent1`.`count`, `Extent1`.`reserved`, `Extent1`.`box`, `Extent1`.`box_name`, `Extent1`.`price_option`, `Extent1`.`currency_rate`, `Extent1`.`in_price`, 
`Extent1`.`price`, `Extent1`.`price2`, `Extent1`.`price3`, `Extent1`.`price4`, `Extent1`.`price5`, `Extent1`.`document`, `Extent1`.`shop_title`, `Extent1`.`shop_description`, 
`Extent1`.`SN`, `Extent1`.`PN`, `Extent1`.`description`, `Extent1`.`shop_enable`, `Extent1`.`int_barcode`, `Extent1`.`ext_barcode`, `Extent1`.`in_summ`, `Extent1`.`notes`, `Extent1`.`img1`,
 `Extent1`.`img2`, `Extent1`.`img3`, `Extent1`.`img4`, `Extent1`.`img5`, `Extent1`.`minimum_in_stock`, `Extent1`.`sold`, `Extent1`.`return_percent`, `Extent1`.`warranty`, 
`Extent1`.`warranty_dealer`, `Extent1`.`in_count`, `Extent1`.`not_for_sale`, `Extent1`.`part_request`, `Extent1`.`st_state`, `Extent1`.`st_notes`, `Extent1`.`units`, 
`Extent1`.`ge_highlight`, `Extent1`.`Hidden`, `Extent1`.`updated`
FROM `store_items` AS `Extent1`
 WHERE `Extent1`.`id` = 27131 LIMIT 1;

SELECT
`Extent1`.`id`, `Extent1`.`item_id`, `Extent1`.`count`, `Extent1`.`created`, `Extent1`.`from_user`, `Extent1`.`to_user`, `Extent1`.`notes`, `Extent1`.`state`, `Extent1`.`repair_id`, 
`Extent1`.`price`, `Extent1`.`sn`, `Extent1`.`work_id`, `Extent1`.`warranty`, `Extent1`.`name`, `Extent1`.`r_lock`
FROM `store_int_reserve` AS `Extent1`
 WHERE `Extent1`.`id` = 19045 LIMIT 2;

BEGIN;

UPDATE `store_int_reserve` SET `state`=3 WHERE `id` = 19045;

COMMIT;

BEGIN;

UPDATE `store_items` SET `count`=20, `reserved`=0, `sold`=30 WHERE `id` = 27131;

COMMIT;

BEGIN;

INSERT INTO `logs`(
`group`, `type`, `arh`, `user`, `created`, `values`, `values_after`, `office`, `client`, `repair`, `item`, `document`, `cash_order`, `part_request`, `notes`) VALUES (
NULL, 6, 0, 32, '2022-03-24 20:53:29.347123', NULL, NULL, 1, 1153, 24882, 27131, NULL, NULL, NULL, 'Товар установленный в ремонт 024882 продан. Ремонт выдан');

SELECT
`id`
FROM `logs`
 WHERE  row_count() > 0 AND `id`=last_insert_id();

COMMIT;

SELECT
`Extent1`.`id`, `Extent1`.`client`, `Extent1`.`type`, `Extent1`.`maker`, `Extent1`.`model`, `Extent1`.`serial_number`, `Extent1`.`company`, `Extent1`.`office`, `Extent1`.`start_office`, 
`Extent1`.`manager`, `Extent1`.`current_manager`, `Extent1`.`master`, `Extent1`.`diagnostic_result`, `Extent1`.`in_date`, `Extent1`.`out_date`, `Extent1`.`state`, `Extent1`.`user_lock`, 
`Extent1`.`lock_datetime`, `Extent1`.`express_repair`, `Extent1`.`is_warranty`, `Extent1`.`is_repeat`, `Extent1`.`can_format`, `Extent1`.`print_check`, `Extent1`.`box`, 
`Extent1`.`warranty_label`, `Extent1`.`ext_notes`, `Extent1`.`is_prepaid`, `Extent1`.`prepaid_type`, `Extent1`.`prepaid_summ`, `Extent1`.`prepaid_order`, `Extent1`.`is_pre_agreed`, 
`Extent1`.`pre_agreed_amount`, `Extent1`.`repair_cost`, `Extent1`.`fault`, `Extent1`.`complect`, `Extent1`.`look`, `Extent1`.`thirs_party_sc`, `Extent1`.`last_save`, 
`Extent1`.`last_status_changed`, `Extent1`.`warranty_days`, `Extent1`.`barcode`, `Extent1`.`new_state`, `Extent1`.`is_card_payment`, `Extent1`.`informed_status`, `Extent1`.`parts_cost`, 
`Extent1`.`is_debt`, `Extent1`.`image_ids`, `Extent1`.`color`, `Extent1`.`order_moving`, `Extent1`.`payment_system`, `Extent1`.`early`, `Extent1`.`real_repair_cost`, `Extent1`.`issued_msg`,
 `Extent1`.`reject_reason`, `Extent1`.`ext_early`, `Extent1`.`sms_inform`, `Extent1`.`invoice`, `Extent1`.`cartridge`, `Extent1`.`termsControl`, `Extent1`.`vendor_id`, 
`Extent1`.`quick_repair`, `Extent1`.`Hidden`, `Extent1`.`Title`
FROM `workshop` AS `Extent1`
 WHERE `Extent1`.`id` = 24882;

SELECT UTC_TIMESTAMP() FROM DUAL;

BEGIN;

UPDATE `workshop` SET `out_date`='2022-03-24 20:53:29', `state`=8, `lock_datetime`='2022-03-24 20:53:01', `last_status_changed`='2022-03-24 20:53:29' WHERE `id` = 24882;

INSERT INTO `workshop_issued`(
`repair_id`, 
`employee_id`, 
`created_at`) VALUES (
24882, 
32, 
'2022-03-24 20:53:29');
SELECT
`id`
FROM `workshop_issued`
 WHERE  row_count() > 0 AND `id`=last_insert_id();

COMMIT;

BEGIN;

INSERT INTO `logs`(
`group`, `type`, `arh`, `user`, `created`, `values`, `values_after`, `office`, `client`, `repair`, `item`, `document`, `cash_order`, `part_request`, `notes`) VALUES (
NULL, 3, 0, 32, '2022-03-24 20:53:29.363159', NULL, NULL, 1, NULL, 24882, NULL, NULL, NULL, NULL, 'Заказаз-наряд: 024882, выдан клиенту. Сумма: 999,99 грн., оплата: Наличные');
SELECT
`id`
FROM `logs`
 WHERE  row_count() > 0 AND `id`=last_insert_id();

COMMIT;

SELECT UTC_TIMESTAMP() FROM DUAL;

BEGIN;

INSERT INTO `repair_status_logs`(
`created_at`, 
`repair_id`, 
`status_id`, 
`user_id`, 
`manager_id`, 
`master_id`) VALUES (
'2022-03-24 20:53:29', 
24882, 
6, 
32, 
33, 
33);
SELECT
`id`
FROM `repair_status_logs`
 WHERE  row_count() > 0 AND `id`=last_insert_id();

COMMIT;

BEGIN;

SELECT
`Extent1`.`id`, `Extent1`.`creator`, `Extent1`.`name`, `Extent1`.`surname`, `Extent1`.`patronymic`, `Extent1`.`agent_phone_mask`, `Extent1`.`agent2_phone_mask`, `Extent1`.`address`, 
`Extent1`.`post_index`, `Extent1`.`passport_num`, `Extent1`.`passport_date`, `Extent1`.`passport_organ`, `Extent1`.`state`, `Extent1`.`type`, `Extent1`.`birthday`, `Extent1`.`memorial`, 
`Extent1`.`notes`, `Extent1`.`is_regular`, `Extent1`.`is_dealer`, `Extent1`.`balance_enable`, `Extent1`.`prefer_cashless`, `Extent1`.`take_long`, `Extent1`.`ignore_calls`, 
`Extent1`.`is_bad`, `Extent1`.`is_realizator`, `Extent1`.`is_agent`, `Extent1`.`INN`, `Extent1`.`KPP`, `Extent1`.`OGRN`, `Extent1`.`web_password`, `Extent1`.`ur_name`, `Extent1`.`email`, 
`Extent1`.`icq`, `Extent1`.`skype`, `Extent1`.`viber`, `Extent1`.`telegram`, `Extent1`.`site`, `Extent1`.`whatsapp`, `Extent1`.`agent_name`, `Extent1`.`agent_surname`, 
`Extent1`.`agent_patronymic`, `Extent1`.`agent_phone`, `Extent1`.`agent2_name`, `Extent1`.`agent2_surname`, `Extent1`.`agent2_patronymic`, `Extent1`.`agent2_phone`, `Extent1`.`created`, 
`Extent1`.`balance`, `Extent1`.`price_col`, `Extent1`.`agent_phone_clean`, `Extent1`.`agent2_phone_clean`, `Extent1`.`repairs`, `Extent1`.`token`, `Extent1`.`purchases`, 
`Extent1`.`visit_source`, `Extent1`.`photo_id`
FROM `clients` AS `Extent1`
 WHERE `Extent1`.`id` = 1153 LIMIT 2;

UPDATE `clients` SET `balance`=450.0100 WHERE `id` = 1153;

INSERT INTO `balance`(
`client`, `summ`, `direction`, `reason`, `created`, `office`, `uid`, `dealer_payment`) VALUES (1153, -999.9900, 0, 'Списание средств за выданный ремонт 024882', '2022-03-24 20:53:29.421347', 1, 32, NULL);

SELECT
`id`
FROM `balance`
 WHERE  row_count() > 0 AND `id`=last_insert_id();

COMMIT;

SELECT
`Project2`.`id`, `Project2`.`client`, `Project2`.`type`, `Project2`.`maker`, `Project2`.`model`, `Project2`.`serial_number`, `Project2`.`company`, `Project2`.`office`, 
`Project2`.`start_office`, `Project2`.`manager`, `Project2`.`current_manager`, `Project2`.`master`, `Project2`.`diagnostic_result`, `Project2`.`in_date`, `Project2`.`out_date`, 
`Project2`.`state`, `Project2`.`user_lock`, `Project2`.`lock_datetime`, `Project2`.`express_repair`, `Project2`.`is_warranty`, `Project2`.`is_repeat`, `Project2`.`can_format`, 
`Project2`.`print_check`, `Project2`.`box`, `Project2`.`warranty_label`, `Project2`.`ext_notes`, `Project2`.`is_prepaid`, `Project2`.`prepaid_type`, `Project2`.`prepaid_summ`, 
`Project2`.`prepaid_order`, `Project2`.`is_pre_agreed`, `Project2`.`pre_agreed_amount`, `Project2`.`repair_cost`, `Project2`.`fault`, `Project2`.`complect`, `Project2`.`look`, 
`Project2`.`thirs_party_sc`, `Project2`.`last_save`, `Project2`.`last_status_changed`, `Project2`.`warranty_days`, `Project2`.`barcode`, `Project2`.`new_state`, 
`Project2`.`is_card_payment`, `Project2`.`informed_status`, `Project2`.`parts_cost`, `Project2`.`is_debt`, `Project2`.`image_ids`, `Project2`.`color`, `Project2`.`order_moving`, 
`Project2`.`payment_system`, `Project2`.`early`, `Project2`.`real_repair_cost`, `Project2`.`issued_msg`, `Project2`.`reject_reason`, `Project2`.`ext_early`, `Project2`.`sms_inform`, 
`Project2`.`invoice`, `Project2`.`cartridge`, `Project2`.`termsControl`, `Project2`.`vendor_id`, `Project2`.`quick_repair`, `Project2`.`Hidden`, `Project2`.`Title`, `Project2`.`id1`, 
`Project2`.`sip_user_id`, `Project2`.`username`, `Project2`.`name`, `Project2`.`surname`, `Project2`.`patronymic`, `Project2`.`phone`, `Project2`.`phone2`, `Project2`.`phone_mask`, 
`Project2`.`phone2_mask`, `Project2`.`address`, `Project2`.`passport_num`, `Project2`.`passport_date`, `Project2`.`passport_organ`, `Project2`.`state1`, `Project2`.`created`, 
`Project2`.`office1`, `Project2`.`birthday`, `Project2`.`last_login`, `Project2`.`last_activity`, `Project2`.`email`, `Project2`.`sex`, `Project2`.`photo`, `Project2`.`salary_rate`, 
`Project2`.`pay_day`, `Project2`.`pay_day_off`, `Project2`.`pay_repair`, `Project2`.`pay_sale`, `Project2`.`pay_repair_q_sale`, `Project2`.`pay_cartridge_refill`, 
`Project2`.`color_label_ws`, `Project2`.`workspace_mode`, `Project2`.`preview_before_print`, `Project2`.`new_rep_doc_copies`, `Project2`.`auto_refresh_workspace`, `Project2`.`refresh_time`,
 `Project2`.`xls_c1`, `Project2`.`xls_c2`, `Project2`.`xls_c3`, `Project2`.`xls_c4`, `Project2`.`xls_c5`, `Project2`.`xls_c6`, `Project2`.`xls_c7`, `Project2`.`xls_c8`, `Project2`.`xls_c9`,
 `Project2`.`xls_c10`, `Project2`.`xls_c11`, `Project2`.`xls_c12`, `Project2`.`xls_c13`, `Project2`.`xls_c14`, `Project2`.`xls_c15`, `Project2`.`display_out`, `Project2`.`display_complete`,
 `Project2`.`is_bot`, `Project2`.`new_on_top`, `Project2`.`issued_color`, `Project2`.`fields_cfg`, `Project2`.`kkm_pass`, `Project2`.`prefer_regular`, `Project2`.`fontsize`, 
`Project2`.`rowheight`, `Project2`.`animation`, `Project2`.`pay_device_in`, `Project2`.`pay_device_out`, `Project2`.`pay_4_sale_in_repair`, `Project2`.`row_color`, 
`Project2`.`save_state_on_close`, `Project2`.`group_store_items`, `Project2`.`track_activity`, `Project2`.`inn`, `Project2`.`inform_comment`, `Project2`.`inform_status`, `Project2`.`kkt`, 
`Project2`.`pinpad`, `Project2`.`def_office`, `Project2`.`def_store`, `Project2`.`def_item_state`, `Project2`.`def_employee`, `Project2`.`def_status`, `Project2`.`def_ws_filter`, 
`Project2`.`card_on_call`, `Project2`.`ge_highlight_color`, `Project2`.`pay_repair_quick`, `Project2`.`advance_disable`, `Project2`.`salary_disable`, `Project2`.`notes`, `Project2`.`id2`, 
`Project2`.`creator`, `Project2`.`name1`, `Project2`.`surname1`, `Project2`.`patronymic1`, `Project2`.`agent_phone_mask`, `Project2`.`agent2_phone_mask`, `Project2`.`address1`, 
`Project2`.`post_index`, `Project2`.`passport_num1`, `Project2`.`passport_date1`, `Project2`.`passport_organ1`, `Project2`.`state2`, `Project2`.`type1`, `Project2`.`birthday1`, 
`Project2`.`memorial`, `Project2`.`notes1`, `Project2`.`is_regular`, `Project2`.`is_dealer`, `Project2`.`balance_enable`, `Project2`.`prefer_cashless`, `Project2`.`take_long`, 
`Project2`.`ignore_calls`, `Project2`.`is_bad`, `Project2`.`is_realizator`, `Project2`.`is_agent`, `Project2`.`inn1`, `Project2`.`KPP`, `Project2`.`OGRN`, `Project2`.`web_password`, 
`Project2`.`ur_name`, `Project2`.`email1`, `Project2`.`icq`, `Project2`.`skype`, `Project2`.`viber`, `Project2`.`telegram`, `Project2`.`site`, `Project2`.`whatsapp`, 
`Project2`.`agent_name`, `Project2`.`agent_surname`, `Project2`.`agent_patronymic`, `Project2`.`agent_phone`, `Project2`.`agent2_name`, `Project2`.`agent2_surname`, 
`Project2`.`agent2_patronymic`, `Project2`.`agent2_phone`, `Project2`.`created1`, `Project2`.`balance`, `Project2`.`price_col`, `Project2`.`agent_phone_clean`, 
`Project2`.`agent2_phone_clean`, `Project2`.`repairs`, `Project2`.`token`, `Project2`.`purchases`, `Project2`.`visit_source`, `Project2`.`photo_id`, `Project2`.`id3`, `Project2`.`num`, 
`Project2`.`created2`, `Project2`.`user`, `Project2`.`seller`, `Project2`.`customer`, `Project2`.`paid`, `Project2`.`tax`, `Project2`.`summ`, `Project2`.`total`, `Project2`.`state3`, 
`Project2`.`office2`, `Project2`.`type2`, `Project2`.`notes2`, `Project2`.`id4`, `Project2`.`refill`, `Project2`.`chip`, `Project2`.`opc_drum`, `Project2`.`c_blade`, `Project2`.`card_id`, 
`Project2`.`id5`, `Project2`.`name2`, `Project2`.`maker1`, `Project2`.`full_weight`, `Project2`.`toner_weight`, `Project2`.`resource`, `Project2`.`created3`, `Project2`.`user1`, 
`Project2`.`notes3`, `Project2`.`photo1`, `Project2`.`color1`, `Project2`.`archive`, `Project2`.`C1`, `Project2`.`id6`, `Project2`.`field_id`, `Project2`.`repair_id`, `Project2`.`value`, 
`Project2`.`item_id`, `Project2`.`id7`, `Project2`.`name3`, `Project2`.`type3`, `Project2`.`def_values`, `Project2`.`required`, `Project2`.`printable`, `Project2`.`archive1`, 
`Project2`.`_f`, `Project2`.`updated_at`, `Project2`.`devices`, `Project2`.`categories`
FROM ( SELECT
`Limit1`.`id`, `Limit1`.`Hidden`, `Limit1`.`Title`, `Limit1`.`client`, `Limit1`.`type`, `Limit1`.`maker`, `Limit1`.`model`, `Limit1`.`serial_number`, `Limit1`.`company`, `Limit1`.`office`,
 `Limit1`.`start_office`, `Limit1`.`manager`, `Limit1`.`current_manager`, `Limit1`.`master`, `Limit1`.`diagnostic_result`, `Limit1`.`in_date`, `Limit1`.`out_date`, `Limit1`.`state`, 
`Limit1`.`new_state`, `Limit1`.`user_lock`, `Limit1`.`lock_datetime`, `Limit1`.`express_repair`, `Limit1`.`quick_repair`, `Limit1`.`is_warranty`, `Limit1`.`is_repeat`, 
`Limit1`.`payment_system`, `Limit1`.`is_card_payment`, `Limit1`.`can_format`, `Limit1`.`print_check`, `Limit1`.`box`, `Limit1`.`warranty_label`, `Limit1`.`ext_notes`, 
`Limit1`.`is_prepaid`, `Limit1`.`prepaid_type`, `Limit1`.`prepaid_summ`, `Limit1`.`prepaid_order`, `Limit1`.`is_pre_agreed`, `Limit1`.`is_debt`, `Limit1`.`pre_agreed_amount`, 
`Limit1`.`repair_cost`, `Limit1`.`real_repair_cost`, `Limit1`.`parts_cost`, `Limit1`.`fault`, `Limit1`.`complect`, `Limit1`.`look`, `Limit1`.`thirs_party_sc`, `Limit1`.`last_save`, 
`Limit1`.`last_status_changed`, `Limit1`.`warranty_days`, `Limit1`.`barcode`, `Limit1`.`reject_reason`, `Limit1`.`informed_status`, `Limit1`.`image_ids`, `Limit1`.`color`, 
`Limit1`.`order_moving`, `Limit1`.`early`, `Limit1`.`ext_early`, `Limit1`.`issued_msg`, `Limit1`.`sms_inform`, `Limit1`.`invoice`, `Limit1`.`cartridge`, `Limit1`.`vendor_id`, 
`Limit1`.`termsControl`, `Limit1`.`id1`, `Limit1`.`sip_user_id`, `Limit1`.`username`, `Limit1`.`name`, `Limit1`.`surname`, `Limit1`.`patronymic`, `Limit1`.`phone`, `Limit1`.`phone2`, 
`Limit1`.`phone_mask`, `Limit1`.`phone2_mask`, `Limit1`.`address`, `Limit1`.`passport_num`, `Limit1`.`passport_date`, `Limit1`.`passport_organ`, `Limit1`.`state1`, `Limit1`.`created`, 
`Limit1`.`office1`, `Limit1`.`birthday`, `Limit1`.`def_office`, `Limit1`.`def_store`, `Limit1`.`def_item_state`, `Limit1`.`def_employee`, `Limit1`.`def_status`, `Limit1`.`def_ws_filter`, 
`Limit1`.`last_login`, `Limit1`.`last_activity`, `Limit1`.`email`, `Limit1`.`sex`, `Limit1`.`photo`, `Limit1`.`salary_rate`, `Limit1`.`pay_day`, `Limit1`.`pay_day_off`, 
`Limit1`.`pay_repair`, `Limit1`.`pay_repair_quick`, `Limit1`.`pay_sale`, `Limit1`.`pay_repair_q_sale`, `Limit1`.`pay_cartridge_refill`, `Limit1`.`pay_device_in`, `Limit1`.`pay_device_out`,
 `Limit1`.`pay_4_sale_in_repair`, `Limit1`.`row_color`, `Limit1`.`ge_highlight_color`, `Limit1`.`color_label_ws`, `Limit1`.`workspace_mode`, `Limit1`.`preview_before_print`, 
`Limit1`.`new_rep_doc_copies`, `Limit1`.`auto_refresh_workspace`, `Limit1`.`refresh_time`, `Limit1`.`xls_c1`, `Limit1`.`xls_c2`, `Limit1`.`xls_c3`, `Limit1`.`xls_c4`, `Limit1`.`xls_c5`, 
`Limit1`.`xls_c6`, `Limit1`.`xls_c7`, `Limit1`.`xls_c8`, `Limit1`.`xls_c9`, `Limit1`.`xls_c10`, `Limit1`.`xls_c11`, `Limit1`.`xls_c12`, `Limit1`.`xls_c13`, `Limit1`.`xls_c14`, 
`Limit1`.`xls_c15`, `Limit1`.`display_out`, `Limit1`.`display_complete`, `Limit1`.`is_bot`, `Limit1`.`new_on_top`, `Limit1`.`issued_color`, `Limit1`.`kkm_pass`, `Limit1`.`prefer_regular`, 
`Limit1`.`fontsize`, `Limit1`.`rowheight`, `Limit1`.`animation`, `Limit1`.`fields_cfg`, `Limit1`.`save_state_on_close`, `Limit1`.`group_store_items`, `Limit1`.`track_activity`, 
`Limit1`.`card_on_call`, `Limit1`.`inn`, `Limit1`.`inform_comment`, `Limit1`.`inform_status`, `Limit1`.`kkt`, `Limit1`.`pinpad`, `Limit1`.`advance_disable`, `Limit1`.`salary_disable`, 
`Limit1`.`notes`, `Limit1`.`id2`, `Limit1`.`creator`, `Limit1`.`name1`, `Limit1`.`surname1`, `Limit1`.`patronymic1`, `Limit1`.`agent_phone_mask`, `Limit1`.`agent2_phone_mask`, 
`Limit1`.`address1`, `Limit1`.`post_index`, `Limit1`.`passport_num1`, `Limit1`.`passport_date1`, `Limit1`.`passport_organ1`, `Limit1`.`state2`, `Limit1`.`type1`, `Limit1`.`birthday1`, 
`Limit1`.`memorial`, `Limit1`.`notes1`, `Limit1`.`is_regular`, `Limit1`.`is_dealer`, `Limit1`.`balance_enable`, `Limit1`.`prefer_cashless`, `Limit1`.`take_long`, `Limit1`.`ignore_calls`, 
`Limit1`.`is_bad`, `Limit1`.`is_realizator`, `Limit1`.`is_agent`, `Limit1`.`visit_source`, `Limit1`.`photo_id`, `Limit1`.`inn1`, `Limit1`.`KPP`, `Limit1`.`OGRN`, `Limit1`.`web_password`, 
`Limit1`.`ur_name`, `Limit1`.`email1`, `Limit1`.`icq`, `Limit1`.`skype`, `Limit1`.`viber`, `Limit1`.`telegram`, `Limit1`.`site`, `Limit1`.`whatsapp`, `Limit1`.`agent_name`, 
`Limit1`.`agent_surname`, `Limit1`.`agent_patronymic`, `Limit1`.`agent_phone`, `Limit1`.`agent_phone_clean`, `Limit1`.`agent2_name`, `Limit1`.`agent2_surname`, 
`Limit1`.`agent2_patronymic`, `Limit1`.`agent2_phone`, `Limit1`.`agent2_phone_clean`, `Limit1`.`created1`, `Limit1`.`balance`, `Limit1`.`price_col`, `Limit1`.`repairs`, 
`Limit1`.`purchases`, `Limit1`.`token`, `Limit1`.`id3`, `Limit1`.`num`, `Limit1`.`created2`, `Limit1`.`user`, `Limit1`.`seller`, `Limit1`.`customer`, `Limit1`.`paid`, `Limit1`.`tax`, 
`Limit1`.`summ`, `Limit1`.`total`, `Limit1`.`state3`, `Limit1`.`office2`, `Limit1`.`type2`, `Limit1`.`notes2`, `Limit1`.`id4`, `Limit1`.`refill`, `Limit1`.`chip`, `Limit1`.`opc_drum`, 
`Limit1`.`c_blade`, `Limit1`.`card_id`, `Limit1`.`id5`, `Limit1`.`name2`, `Limit1`.`maker1`, `Limit1`.`full_weight`, `Limit1`.`toner_weight`, `Limit1`.`resource`, `Limit1`.`created3`, 
`Limit1`.`user1`, `Limit1`.`notes3`, `Limit1`.`photo1`, `Limit1`.`color1`, `Limit1`.`archive`, `Join6`.`id` AS `id6`, `Join6`.`field_id`, `Join6`.`repair_id`, `Join6`.`item_id`, 
`Join6`.`value`, `Join6`.`ID1` AS `id7`, `Join6`.`_f`, `Join6`.`name` AS `name3`, `Join6`.`type` AS `type3`, `Join6`.`def_values`, `Join6`.`required`, `Join6`.`printable`, 
`Join6`.`devices`, `Join6`.`categories`, `Join6`.`updated_at`, `Join6`.`archive` AS `archive1`, CASE WHEN (`Join6`.`id` IS NOT NULL) THEN (1)  ELSE (NULL) END AS `C1`
FROM ( SELECT
`Extent1`.`id`, `Extent1`.`Hidden`, `Extent1`.`Title`, `Extent1`.`client`, `Extent1`.`type`, `Extent1`.`maker`, `Extent1`.`model`, `Extent1`.`serial_number`, `Extent1`.`company`, 
`Extent1`.`office`, `Extent1`.`start_office`, `Extent1`.`manager`, `Extent1`.`current_manager`, `Extent1`.`master`, `Extent1`.`diagnostic_result`, `Extent1`.`in_date`, 
`Extent1`.`out_date`, `Extent1`.`state`, `Extent1`.`new_state`, `Extent1`.`user_lock`, `Extent1`.`lock_datetime`, `Extent1`.`express_repair`, `Extent1`.`quick_repair`, 
`Extent1`.`is_warranty`, `Extent1`.`is_repeat`, `Extent1`.`payment_system`, `Extent1`.`is_card_payment`, `Extent1`.`can_format`, `Extent1`.`print_check`, `Extent1`.`box`, 
`Extent1`.`warranty_label`, `Extent1`.`ext_notes`, `Extent1`.`is_prepaid`, `Extent1`.`prepaid_type`, `Extent1`.`prepaid_summ`, `Extent1`.`prepaid_order`, `Extent1`.`is_pre_agreed`, 
`Extent1`.`is_debt`, `Extent1`.`pre_agreed_amount`, `Extent1`.`repair_cost`, `Extent1`.`real_repair_cost`, `Extent1`.`parts_cost`, `Extent1`.`fault`, `Extent1`.`complect`, 
`Extent1`.`look`, `Extent1`.`thirs_party_sc`, `Extent1`.`last_save`, `Extent1`.`last_status_changed`, `Extent1`.`warranty_days`, `Extent1`.`barcode`, `Extent1`.`reject_reason`, 
`Extent1`.`informed_status`, `Extent1`.`image_ids`, `Extent1`.`color`, `Extent1`.`order_moving`, `Extent1`.`early`, `Extent1`.`ext_early`, `Extent1`.`issued_msg`, `Extent1`.`sms_inform`, 
`Extent1`.`invoice`, `Extent1`.`cartridge`, `Extent1`.`vendor_id`, `Extent1`.`termsControl`, `Extent2`.`id` AS `id1`, `Extent2`.`sip_user_id`, `Extent2`.`username`, `Extent2`.`name`, 
`Extent2`.`surname`, `Extent2`.`patronymic`, `Extent2`.`phone`, `Extent2`.`phone2`, `Extent2`.`phone_mask`, `Extent2`.`phone2_mask`, `Extent2`.`address`, `Extent2`.`passport_num`, 
`Extent2`.`passport_date`, `Extent2`.`passport_organ`, `Extent2`.`state` AS `state1`, `Extent2`.`created`, `Extent2`.`office` AS `office1`, `Extent2`.`birthday`, `Extent2`.`def_office`, 
`Extent2`.`def_store`, `Extent2`.`def_item_state`, `Extent2`.`def_employee`, `Extent2`.`def_status`, `Extent2`.`def_ws_filter`, `Extent2`.`last_login`, `Extent2`.`last_activity`, 
`Extent2`.`email`, `Extent2`.`sex`, `Extent2`.`photo`, `Extent2`.`salary_rate`, `Extent2`.`pay_day`, `Extent2`.`pay_day_off`, `Extent2`.`pay_repair`, `Extent2`.`pay_repair_quick`, 
`Extent2`.`pay_sale`, `Extent2`.`pay_repair_q_sale`, `Extent2`.`pay_cartridge_refill`, `Extent2`.`pay_device_in`, `Extent2`.`pay_device_out`, `Extent2`.`pay_4_sale_in_repair`, 
`Extent2`.`row_color`, `Extent2`.`ge_highlight_color`, `Extent2`.`color_label_ws`, `Extent2`.`workspace_mode`, `Extent2`.`preview_before_print`, `Extent2`.`new_rep_doc_copies`, 
`Extent2`.`auto_refresh_workspace`, `Extent2`.`refresh_time`, `Extent2`.`xls_c1`, `Extent2`.`xls_c2`, `Extent2`.`xls_c3`, `Extent2`.`xls_c4`, `Extent2`.`xls_c5`, `Extent2`.`xls_c6`, 
`Extent2`.`xls_c7`, `Extent2`.`xls_c8`, `Extent2`.`xls_c9`, `Extent2`.`xls_c10`, `Extent2`.`xls_c11`, `Extent2`.`xls_c12`, `Extent2`.`xls_c13`, `Extent2`.`xls_c14`, `Extent2`.`xls_c15`, 
`Extent2`.`display_out`, `Extent2`.`display_complete`, `Extent2`.`is_bot`, `Extent2`.`new_on_top`, `Extent2`.`issued_color`, `Extent2`.`kkm_pass`, `Extent2`.`prefer_regular`, 
`Extent2`.`fontsize`, `Extent2`.`rowheight`, `Extent2`.`animation`, `Extent2`.`fields_cfg`, `Extent2`.`save_state_on_close`, `Extent2`.`group_store_items`, `Extent2`.`track_activity`, 
`Extent2`.`card_on_call`, `Extent2`.`inn`, `Extent2`.`inform_comment`, `Extent2`.`inform_status`, `Extent2`.`kkt`, `Extent2`.`pinpad`, `Extent2`.`advance_disable`, 
`Extent2`.`salary_disable`, `Extent2`.`notes`, `Extent3`.`id` AS `id2`, `Extent3`.`creator`, `Extent3`.`name` AS `name1`, `Extent3`.`surname` AS `surname1`, `Extent3`.`patronymic` AS 
`patronymic1`, `Extent3`.`agent_phone_mask`, `Extent3`.`agent2_phone_mask`, `Extent3`.`address` AS `address1`, `Extent3`.`post_index`, `Extent3`.`passport_num` AS `passport_num1`, 
`Extent3`.`passport_date` AS `passport_date1`, `Extent3`.`passport_organ` AS `passport_organ1`, `Extent3`.`state` AS `state2`, `Extent3`.`type` AS `type1`, `Extent3`.`birthday` AS 
`birthday1`, `Extent3`.`memorial`, `Extent3`.`notes` AS `notes1`, `Extent3`.`is_regular`, `Extent3`.`is_dealer`, `Extent3`.`balance_enable`, `Extent3`.`prefer_cashless`, 
`Extent3`.`take_long`, `Extent3`.`ignore_calls`, `Extent3`.`is_bad`, `Extent3`.`is_realizator`, `Extent3`.`is_agent`, `Extent3`.`visit_source`, `Extent3`.`photo_id`, `Extent3`.`INN` AS 
`inn1`, `Extent3`.`KPP`, `Extent3`.`OGRN`, `Extent3`.`web_password`, `Extent3`.`ur_name`, `Extent3`.`email` AS `email1`, `Extent3`.`icq`, `Extent3`.`skype`, `Extent3`.`viber`, 
`Extent3`.`telegram`, `Extent3`.`site`, `Extent3`.`whatsapp`, `Extent3`.`agent_name`, `Extent3`.`agent_surname`, `Extent3`.`agent_patronymic`, `Extent3`.`agent_phone`, 
`Extent3`.`agent_phone_clean`, `Extent3`.`agent2_name`, `Extent3`.`agent2_surname`, `Extent3`.`agent2_patronymic`, `Extent3`.`agent2_phone`, `Extent3`.`agent2_phone_clean`, 
`Extent3`.`created` AS `created1`, `Extent3`.`balance`, `Extent3`.`price_col`, `Extent3`.`repairs`, `Extent3`.`purchases`, `Extent3`.`token`, `Extent4`.`id` AS `id3`, `Extent4`.`num`, 
`Extent4`.`created` AS `created2`, `Extent4`.`user`, `Extent4`.`seller`, `Extent4`.`customer`, `Extent4`.`paid`, `Extent4`.`tax`, `Extent4`.`summ`, `Extent4`.`total`, `Extent4`.`state` AS 
`state3`, `Extent4`.`office` AS `office2`, `Extent4`.`type` AS `type2`, `Extent4`.`notes` AS `notes2`, `Extent5`.`id` AS `id4`, `Extent5`.`refill`, `Extent5`.`chip`, `Extent5`.`opc_drum`, 
`Extent5`.`c_blade`, `Extent5`.`card_id`, `Extent6`.`id` AS `id5`, `Extent6`.`name` AS `name2`, `Extent6`.`maker` AS `maker1`, `Extent6`.`full_weight`, `Extent6`.`toner_weight`, 
`Extent6`.`resource`, `Extent6`.`created` AS `created3`, `Extent6`.`user` AS `user1`, `Extent6`.`notes` AS `notes3`, `Extent6`.`photo` AS `photo1`, `Extent6`.`color` AS `color1`, 
`Extent6`.`archive`
FROM `workshop` AS `Extent1` LEFT OUTER JOIN `users` AS `Extent2` ON `Extent1`.`user_lock` = `Extent2`.`id` INNER JOIN `clients` AS `Extent3` ON `Extent1`.`client` = `Extent3`.`id` LEFT OUTER JOIN `invoice` AS `Extent4` ON `Extent1`.`invoice` = `Extent4`.`id` LEFT OUTER JOIN `c_workshop` AS `Extent5` ON `Extent1`.`cartridge` = `Extent5`.`id` LEFT OUTER JOIN `cartridge_cards` AS `Extent6` ON `Extent5`.`card_id` = `Extent6`.`id`
 WHERE `Extent1`.`id` = 24882 LIMIT 2) AS `Limit1` LEFT OUTER JOIN (SELECT
`Extent7`.`id`, `Extent7`.`field_id`, `Extent7`.`repair_id`, `Extent7`.`item_id`, `Extent7`.`value`, `Extent8`.`id` AS `ID1`, `Extent8`.`_f`, `Extent8`.`name`, `Extent8`.`type`, `Extent8`.`def_values`, `Extent8`.`required`, `Extent8`.`printable`, `Extent8`.`devices`, `Extent8`.`categories`, `Extent8`.`updated_at`, `Extent8`.`archive`
FROM `field_values` AS `Extent7` INNER JOIN `fields` AS `Extent8` ON `Extent7`.`field_id` = `Extent8`.`id`) AS `Join6` ON `Limit1`.`id` = `Join6`.`repair_id`) AS `Project2`
 ORDER BY 
`Project2`.`id` ASC, `Project2`.`id1` ASC, `Project2`.`id2` ASC, `Project2`.`id3` ASC, `Project2`.`id4` ASC, `Project2`.`id5` ASC, `Project2`.`C1` ASC;

SELECT
`GroupBy1`.`A1` AS `C1`
FROM (SELECT
COUNT(1) AS `A1`
FROM `hooks` AS `Extent1`
 WHERE ((1 = `Extent1`.`hook_id`) AND ('NOTIFY_INTERNAL' = `Extent1`.`event`)) AND ((`Extent1`.`p0` = 6032) OR ((`Extent1`.`p0` IS  NULL) AND (6032 IS  NULL)))) AS `GroupBy1`;

SELECT
`GroupBy1`.`A1` AS `C1`
FROM (SELECT
COUNT(1) AS `A1`
FROM `notifications` AS `Extent1`
 WHERE (`Extent1`.`status` != 1) AND (`Extent1`.`employee` = 32)) AS `GroupBy1`;

SELECT
`GroupBy1`.`A1` AS `C1`
FROM (SELECT
COUNT(1) AS `A1`
FROM `notifications` AS `Extent1`
 WHERE (`Extent1`.`status` != 1) AND (`Extent1`.`employee` = 32)) AS `GroupBy1`;

SELECT
`GroupBy1`.`A1` AS `C1`
FROM (SELECT
COUNT(1) AS `A1`
FROM `notifications` AS `Extent1`
 WHERE (`Extent1`.`status` != 1) AND (`Extent1`.`employee` = 32)) AS `GroupBy1`;

SELECT
`GroupBy1`.`A1` AS `C1`
FROM (SELECT
COUNT(1) AS `A1`
FROM `notifications` AS `Extent1`
 WHERE (`Extent1`.`status` != 1) AND (`Extent1`.`employee` = 32)) AS `GroupBy1`;

SELECT
`GroupBy1`.`A1` AS `C1`
FROM (SELECT
COUNT(1) AS `A1`
FROM `hooks` AS `Extent1`
 WHERE ((1 = `Extent1`.`hook_id`) AND ('NOTIFY_INTERNAL' = `Extent1`.`event`)) AND ((`Extent1`.`p0` = 6032) OR ((`Extent1`.`p0` IS  NULL) AND (6032 IS  NULL)))) AS `GroupBy1`;

BEGIN;

UPDATE `workshop` SET `user_lock`=32, `lock_datetime`='2022-03-24 20:53:31.039678' WHERE `id` = 24882;

COMMIT;

#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# выдача в долг; ремонт с добавленными запчастями (несколько строк).
argument;

SELECT
`Project2`.`id`, `Project2`.`client`, `Project2`.`type`, `Project2`.`maker`, `Project2`.`model`, `Project2`.`serial_number`, `Project2`.`company`, `Project2`.`office`, 
`Project2`.`start_office`, `Project2`.`manager`, `Project2`.`current_manager`, `Project2`.`master`, `Project2`.`diagnostic_result`, `Project2`.`in_date`, `Project2`.`out_date`, 
`Project2`.`state`, `Project2`.`user_lock`, `Project2`.`lock_datetime`, `Project2`.`express_repair`, `Project2`.`is_warranty`, `Project2`.`is_repeat`, `Project2`.`can_format`, 
`Project2`.`print_check`, `Project2`.`box`, `Project2`.`warranty_label`, `Project2`.`ext_notes`, `Project2`.`is_prepaid`, `Project2`.`prepaid_type`, `Project2`.`prepaid_summ`, 
`Project2`.`prepaid_order`, `Project2`.`is_pre_agreed`, `Project2`.`pre_agreed_amount`, `Project2`.`repair_cost`, `Project2`.`fault`, `Project2`.`complect`, `Project2`.`look`, 
`Project2`.`thirs_party_sc`, `Project2`.`last_save`, `Project2`.`last_status_changed`, `Project2`.`warranty_days`, `Project2`.`barcode`, `Project2`.`new_state`, 
`Project2`.`is_card_payment`, `Project2`.`informed_status`, `Project2`.`parts_cost`, `Project2`.`is_debt`, `Project2`.`image_ids`, `Project2`.`color`, `Project2`.`order_moving`, 
`Project2`.`payment_system`, `Project2`.`early`, `Project2`.`real_repair_cost`, `Project2`.`issued_msg`, `Project2`.`reject_reason`, `Project2`.`ext_early`, `Project2`.`sms_inform`, 
`Project2`.`invoice`, `Project2`.`cartridge`, `Project2`.`termsControl`, `Project2`.`vendor_id`, `Project2`.`quick_repair`, `Project2`.`Hidden`, `Project2`.`Title`, `Project2`.`id1`, 
`Project2`.`sip_user_id`, `Project2`.`username`, `Project2`.`name`, `Project2`.`surname`, `Project2`.`patronymic`, `Project2`.`phone`, `Project2`.`phone2`, `Project2`.`phone_mask`, 
`Project2`.`phone2_mask`, `Project2`.`address`, `Project2`.`passport_num`, `Project2`.`passport_date`, `Project2`.`passport_organ`, `Project2`.`state1`, `Project2`.`created`, 
`Project2`.`office1`, `Project2`.`birthday`, `Project2`.`last_login`, `Project2`.`last_activity`, `Project2`.`email`, `Project2`.`sex`, `Project2`.`photo`, `Project2`.`salary_rate`, 
`Project2`.`pay_day`, `Project2`.`pay_day_off`, `Project2`.`pay_repair`, `Project2`.`pay_sale`, `Project2`.`pay_repair_q_sale`, `Project2`.`pay_cartridge_refill`, 
`Project2`.`color_label_ws`, `Project2`.`workspace_mode`, `Project2`.`preview_before_print`, `Project2`.`new_rep_doc_copies`, `Project2`.`auto_refresh_workspace`, 
`Project2`.`refresh_time`, `Project2`.`xls_c1`, `Project2`.`xls_c2`, `Project2`.`xls_c3`, `Project2`.`xls_c4`, `Project2`.`xls_c5`, `Project2`.`xls_c6`, `Project2`.`xls_c7`, 
`Project2`.`xls_c8`, `Project2`.`xls_c9`, `Project2`.`xls_c10`, `Project2`.`xls_c11`, `Project2`.`xls_c12`, `Project2`.`xls_c13`, `Project2`.`xls_c14`, `Project2`.`xls_c15`, 
`Project2`.`display_out`, `Project2`.`display_complete`, `Project2`.`is_bot`, `Project2`.`new_on_top`, `Project2`.`issued_color`, `Project2`.`fields_cfg`, `Project2`.`kkm_pass`, 
`Project2`.`prefer_regular`, `Project2`.`fontsize`, `Project2`.`rowheight`, `Project2`.`animation`, `Project2`.`pay_device_in`, `Project2`.`pay_device_out`, 
`Project2`.`pay_4_sale_in_repair`, `Project2`.`row_color`, `Project2`.`save_state_on_close`, `Project2`.`group_store_items`, `Project2`.`track_activity`, `Project2`.`inn`, 
`Project2`.`inform_comment`, `Project2`.`inform_status`, `Project2`.`kkt`, `Project2`.`pinpad`, `Project2`.`def_office`, `Project2`.`def_store`, `Project2`.`def_item_state`, 
`Project2`.`def_employee`, `Project2`.`def_status`, `Project2`.`def_ws_filter`, `Project2`.`card_on_call`, `Project2`.`ge_highlight_color`, `Project2`.`pay_repair_quick`, 
`Project2`.`advance_disable`, `Project2`.`salary_disable`, `Project2`.`notes`, `Project2`.`id2`, `Project2`.`creator`, `Project2`.`name1`, `Project2`.`surname1`, `Project2`.`patronymic1`, 
`Project2`.`agent_phone_mask`, `Project2`.`agent2_phone_mask`, `Project2`.`address1`, `Project2`.`post_index`, `Project2`.`passport_num1`, `Project2`.`passport_date1`, 
`Project2`.`passport_organ1`, `Project2`.`state2`, `Project2`.`type1`, `Project2`.`birthday1`, `Project2`.`memorial`, `Project2`.`notes1`, `Project2`.`is_regular`, `Project2`.`is_dealer`, 
`Project2`.`balance_enable`, `Project2`.`prefer_cashless`, `Project2`.`take_long`, `Project2`.`ignore_calls`, `Project2`.`is_bad`, `Project2`.`is_realizator`, `Project2`.`is_agent`, 
`Project2`.`inn1`, `Project2`.`KPP`, `Project2`.`OGRN`, `Project2`.`web_password`, `Project2`.`ur_name`, `Project2`.`email1`, `Project2`.`icq`, `Project2`.`skype`, `Project2`.`viber`, 
`Project2`.`telegram`, `Project2`.`site`, `Project2`.`whatsapp`, `Project2`.`agent_name`, `Project2`.`agent_surname`, `Project2`.`agent_patronymic`, `Project2`.`agent_phone`, 
`Project2`.`agent2_name`, `Project2`.`agent2_surname`, `Project2`.`agent2_patronymic`, `Project2`.`agent2_phone`, `Project2`.`created1`, `Project2`.`balance`, `Project2`.`price_col`, 
`Project2`.`agent_phone_clean`, `Project2`.`agent2_phone_clean`, `Project2`.`repairs`, `Project2`.`token`, `Project2`.`purchases`, `Project2`.`visit_source`, `Project2`.`photo_id`, 
`Project2`.`id3`, `Project2`.`num`, `Project2`.`created2`, `Project2`.`user`, `Project2`.`seller`, `Project2`.`customer`, `Project2`.`paid`, `Project2`.`tax`, `Project2`.`summ`, 
`Project2`.`total`, `Project2`.`state3`, `Project2`.`office2`, `Project2`.`type2`, `Project2`.`notes2`, `Project2`.`id4`, `Project2`.`refill`, `Project2`.`chip`, `Project2`.`opc_drum`, 
`Project2`.`c_blade`, `Project2`.`card_id`, `Project2`.`id5`, `Project2`.`name2`, `Project2`.`maker1`, `Project2`.`full_weight`, `Project2`.`toner_weight`, `Project2`.`resource`, 
`Project2`.`created3`, `Project2`.`user1`, `Project2`.`notes3`, `Project2`.`photo1`, `Project2`.`color1`, `Project2`.`archive`, `Project2`.`C1`, `Project2`.`id6`, `Project2`.`field_id`, 
`Project2`.`repair_id`, `Project2`.`value`, `Project2`.`item_id`, `Project2`.`id7`, `Project2`.`name3`, `Project2`.`type3`, `Project2`.`def_values`, `Project2`.`required`, 
`Project2`.`printable`, `Project2`.`archive1`, `Project2`.`_f`, `Project2`.`updated_at`, `Project2`.`devices`, `Project2`.`categories`
FROM (SELECT
`Limit1`.`id`, `Limit1`.`Hidden`, `Limit1`.`Title`, `Limit1`.`client`, `Limit1`.`type`, `Limit1`.`maker`, `Limit1`.`model`, `Limit1`.`serial_number`, `Limit1`.`company`, `Limit1`.`office`,
 `Limit1`.`start_office`, `Limit1`.`manager`, `Limit1`.`current_manager`, `Limit1`.`master`, `Limit1`.`diagnostic_result`, `Limit1`.`in_date`, `Limit1`.`out_date`, `Limit1`.`state`, 
`Limit1`.`new_state`, `Limit1`.`user_lock`, `Limit1`.`lock_datetime`, `Limit1`.`express_repair`, `Limit1`.`quick_repair`, `Limit1`.`is_warranty`, `Limit1`.`is_repeat`, 
`Limit1`.`payment_system`, `Limit1`.`is_card_payment`, `Limit1`.`can_format`, `Limit1`.`print_check`, `Limit1`.`box`, `Limit1`.`warranty_label`, `Limit1`.`ext_notes`, 
`Limit1`.`is_prepaid`, `Limit1`.`prepaid_type`, `Limit1`.`prepaid_summ`, `Limit1`.`prepaid_order`, `Limit1`.`is_pre_agreed`, `Limit1`.`is_debt`, `Limit1`.`pre_agreed_amount`, 
`Limit1`.`repair_cost`, `Limit1`.`real_repair_cost`, `Limit1`.`parts_cost`, `Limit1`.`fault`, `Limit1`.`complect`, `Limit1`.`look`, `Limit1`.`thirs_party_sc`, `Limit1`.`last_save`, 
`Limit1`.`last_status_changed`, `Limit1`.`warranty_days`, `Limit1`.`barcode`, `Limit1`.`reject_reason`, `Limit1`.`informed_status`, `Limit1`.`image_ids`, `Limit1`.`color`, 
`Limit1`.`order_moving`, `Limit1`.`early`, `Limit1`.`ext_early`, `Limit1`.`issued_msg`, `Limit1`.`sms_inform`, `Limit1`.`invoice`, `Limit1`.`cartridge`, `Limit1`.`vendor_id`, 
`Limit1`.`termsControl`, `Limit1`.`id1`, `Limit1`.`sip_user_id`, `Limit1`.`username`, `Limit1`.`name`, `Limit1`.`surname`, `Limit1`.`patronymic`, `Limit1`.`phone`, `Limit1`.`phone2`, 
`Limit1`.`phone_mask`, `Limit1`.`phone2_mask`, `Limit1`.`address`, `Limit1`.`passport_num`, `Limit1`.`passport_date`, `Limit1`.`passport_organ`, `Limit1`.`state1`, `Limit1`.`created`, 
`Limit1`.`office1`, `Limit1`.`birthday`, `Limit1`.`def_office`, `Limit1`.`def_store`, `Limit1`.`def_item_state`, `Limit1`.`def_employee`, `Limit1`.`def_status`, `Limit1`.`def_ws_filter`, 
`Limit1`.`last_login`, `Limit1`.`last_activity`, `Limit1`.`email`, `Limit1`.`sex`, `Limit1`.`photo`, `Limit1`.`salary_rate`, `Limit1`.`pay_day`, `Limit1`.`pay_day_off`, 
`Limit1`.`pay_repair`, `Limit1`.`pay_repair_quick`, `Limit1`.`pay_sale`, `Limit1`.`pay_repair_q_sale`, `Limit1`.`pay_cartridge_refill`, `Limit1`.`pay_device_in`, `Limit1`.`pay_device_out`,
 `Limit1`.`pay_4_sale_in_repair`, `Limit1`.`row_color`, `Limit1`.`ge_highlight_color`, `Limit1`.`color_label_ws`, `Limit1`.`workspace_mode`, `Limit1`.`preview_before_print`, 
`Limit1`.`new_rep_doc_copies`, `Limit1`.`auto_refresh_workspace`, `Limit1`.`refresh_time`, `Limit1`.`xls_c1`, `Limit1`.`xls_c2`, `Limit1`.`xls_c3`, `Limit1`.`xls_c4`, `Limit1`.`xls_c5`, 
`Limit1`.`xls_c6`, `Limit1`.`xls_c7`, `Limit1`.`xls_c8`, `Limit1`.`xls_c9`, `Limit1`.`xls_c10`, `Limit1`.`xls_c11`, `Limit1`.`xls_c12`, `Limit1`.`xls_c13`, `Limit1`.`xls_c14`, 
`Limit1`.`xls_c15`, `Limit1`.`display_out`, `Limit1`.`display_complete`, `Limit1`.`is_bot`, `Limit1`.`new_on_top`, `Limit1`.`issued_color`, `Limit1`.`kkm_pass`, `Limit1`.`prefer_regular`, 
`Limit1`.`fontsize`, `Limit1`.`rowheight`, `Limit1`.`animation`, `Limit1`.`fields_cfg`, `Limit1`.`save_state_on_close`, `Limit1`.`group_store_items`, `Limit1`.`track_activity`, 
`Limit1`.`card_on_call`, `Limit1`.`inn`, `Limit1`.`inform_comment`, `Limit1`.`inform_status`, `Limit1`.`kkt`, `Limit1`.`pinpad`, `Limit1`.`advance_disable`, `Limit1`.`salary_disable`, 
`Limit1`.`notes`, `Limit1`.`id2`, `Limit1`.`creator`, `Limit1`.`name1`, `Limit1`.`surname1`, `Limit1`.`patronymic1`, `Limit1`.`agent_phone_mask`, `Limit1`.`agent2_phone_mask`, 
`Limit1`.`address1`, `Limit1`.`post_index`, `Limit1`.`passport_num1`, `Limit1`.`passport_date1`, `Limit1`.`passport_organ1`, `Limit1`.`state2`, `Limit1`.`type1`, `Limit1`.`birthday1`, 
`Limit1`.`memorial`, `Limit1`.`notes1`, `Limit1`.`is_regular`, `Limit1`.`is_dealer`, `Limit1`.`balance_enable`, `Limit1`.`prefer_cashless`, `Limit1`.`take_long`, `Limit1`.`ignore_calls`, 
`Limit1`.`is_bad`, `Limit1`.`is_realizator`, `Limit1`.`is_agent`, `Limit1`.`visit_source`, `Limit1`.`photo_id`, `Limit1`.`inn1`, `Limit1`.`KPP`, `Limit1`.`OGRN`, `Limit1`.`web_password`, 
`Limit1`.`ur_name`, `Limit1`.`email1`, `Limit1`.`icq`, `Limit1`.`skype`, `Limit1`.`viber`, `Limit1`.`telegram`, `Limit1`.`site`, `Limit1`.`whatsapp`, `Limit1`.`agent_name`, 
`Limit1`.`agent_surname`, `Limit1`.`agent_patronymic`, `Limit1`.`agent_phone`, `Limit1`.`agent_phone_clean`, `Limit1`.`agent2_name`, `Limit1`.`agent2_surname`, 
`Limit1`.`agent2_patronymic`, `Limit1`.`agent2_phone`, `Limit1`.`agent2_phone_clean`, `Limit1`.`created1`, `Limit1`.`balance`, `Limit1`.`price_col`, `Limit1`.`repairs`, 
`Limit1`.`purchases`, `Limit1`.`token`, `Limit1`.`id3`, `Limit1`.`num`, `Limit1`.`created2`, `Limit1`.`user`, `Limit1`.`seller`, `Limit1`.`customer`, `Limit1`.`paid`, `Limit1`.`tax`, 
`Limit1`.`summ`, `Limit1`.`total`, `Limit1`.`state3`, `Limit1`.`office2`, `Limit1`.`type2`, `Limit1`.`notes2`, `Limit1`.`id4`, `Limit1`.`refill`, `Limit1`.`chip`, `Limit1`.`opc_drum`, 
`Limit1`.`c_blade`, `Limit1`.`card_id`, `Limit1`.`id5`, `Limit1`.`name2`, `Limit1`.`maker1`, `Limit1`.`full_weight`, `Limit1`.`toner_weight`, `Limit1`.`resource`, `Limit1`.`created3`, 
`Limit1`.`user1`, `Limit1`.`notes3`, `Limit1`.`photo1`, `Limit1`.`color1`, `Limit1`.`archive`, `Join6`.`id` AS `id6`, `Join6`.`field_id`, `Join6`.`repair_id`, `Join6`.`item_id`, 
`Join6`.`value`, `Join6`.`ID1` AS `id7`, `Join6`.`_f`, `Join6`.`name` AS `name3`, `Join6`.`type` AS `type3`, `Join6`.`def_values`, `Join6`.`required`, `Join6`.`printable`, 
`Join6`.`devices`, `Join6`.`categories`, `Join6`.`updated_at`, `Join6`.`archive` AS `archive1`, CASE WHEN (`Join6`.`id` IS NOT NULL) THEN (1)  ELSE (NULL) END AS `C1`
FROM (SELECT
`Extent1`.`id`, `Extent1`.`Hidden`, `Extent1`.`Title`, `Extent1`.`client`, `Extent1`.`type`, `Extent1`.`maker`, `Extent1`.`model`, `Extent1`.`serial_number`, `Extent1`.`company`, 
`Extent1`.`office`, `Extent1`.`start_office`, `Extent1`.`manager`, `Extent1`.`current_manager`, `Extent1`.`master`, `Extent1`.`diagnostic_result`, `Extent1`.`in_date`, 
`Extent1`.`out_date`, `Extent1`.`state`, `Extent1`.`new_state`, `Extent1`.`user_lock`, `Extent1`.`lock_datetime`, `Extent1`.`express_repair`, `Extent1`.`quick_repair`, 
`Extent1`.`is_warranty`, `Extent1`.`is_repeat`, `Extent1`.`payment_system`, `Extent1`.`is_card_payment`, `Extent1`.`can_format`, `Extent1`.`print_check`, `Extent1`.`box`, 
`Extent1`.`warranty_label`, `Extent1`.`ext_notes`, `Extent1`.`is_prepaid`, `Extent1`.`prepaid_type`, `Extent1`.`prepaid_summ`, `Extent1`.`prepaid_order`, `Extent1`.`is_pre_agreed`, 
`Extent1`.`is_debt`, `Extent1`.`pre_agreed_amount`, `Extent1`.`repair_cost`, `Extent1`.`real_repair_cost`, `Extent1`.`parts_cost`, `Extent1`.`fault`, `Extent1`.`complect`, 
`Extent1`.`look`, `Extent1`.`thirs_party_sc`, `Extent1`.`last_save`, `Extent1`.`last_status_changed`, `Extent1`.`warranty_days`, `Extent1`.`barcode`, `Extent1`.`reject_reason`, 
`Extent1`.`informed_status`, `Extent1`.`image_ids`, `Extent1`.`color`, `Extent1`.`order_moving`, `Extent1`.`early`, `Extent1`.`ext_early`, `Extent1`.`issued_msg`, `Extent1`.`sms_inform`, 
`Extent1`.`invoice`, `Extent1`.`cartridge`, `Extent1`.`vendor_id`, `Extent1`.`termsControl`, `Extent2`.`id` AS `id1`, `Extent2`.`sip_user_id`, `Extent2`.`username`, `Extent2`.`name`, 
`Extent2`.`surname`, `Extent2`.`patronymic`, `Extent2`.`phone`, `Extent2`.`phone2`, `Extent2`.`phone_mask`, `Extent2`.`phone2_mask`, `Extent2`.`address`, `Extent2`.`passport_num`, 
`Extent2`.`passport_date`, `Extent2`.`passport_organ`, `Extent2`.`state` AS `state1`, `Extent2`.`created`, `Extent2`.`office` AS `office1`, `Extent2`.`birthday`, `Extent2`.`def_office`, 
`Extent2`.`def_store`, `Extent2`.`def_item_state`, `Extent2`.`def_employee`, `Extent2`.`def_status`, `Extent2`.`def_ws_filter`, `Extent2`.`last_login`, `Extent2`.`last_activity`, 
`Extent2`.`email`, `Extent2`.`sex`, `Extent2`.`photo`, `Extent2`.`salary_rate`, `Extent2`.`pay_day`, `Extent2`.`pay_day_off`, `Extent2`.`pay_repair`, `Extent2`.`pay_repair_quick`, 
`Extent2`.`pay_sale`, `Extent2`.`pay_repair_q_sale`, `Extent2`.`pay_cartridge_refill`, `Extent2`.`pay_device_in`, `Extent2`.`pay_device_out`, `Extent2`.`pay_4_sale_in_repair`, 
`Extent2`.`row_color`, `Extent2`.`ge_highlight_color`, `Extent2`.`color_label_ws`, `Extent2`.`workspace_mode`, `Extent2`.`preview_before_print`, `Extent2`.`new_rep_doc_copies`, 
`Extent2`.`auto_refresh_workspace`, `Extent2`.`refresh_time`, `Extent2`.`xls_c1`, `Extent2`.`xls_c2`, `Extent2`.`xls_c3`, `Extent2`.`xls_c4`, `Extent2`.`xls_c5`, `Extent2`.`xls_c6`, 
`Extent2`.`xls_c7`, `Extent2`.`xls_c8`, `Extent2`.`xls_c9`, `Extent2`.`xls_c10`, `Extent2`.`xls_c11`, `Extent2`.`xls_c12`, `Extent2`.`xls_c13`, `Extent2`.`xls_c14`, `Extent2`.`xls_c15`, 
`Extent2`.`display_out`, `Extent2`.`display_complete`, `Extent2`.`is_bot`, `Extent2`.`new_on_top`, `Extent2`.`issued_color`, `Extent2`.`kkm_pass`, `Extent2`.`prefer_regular`, 
`Extent2`.`fontsize`, `Extent2`.`rowheight`, `Extent2`.`animation`, `Extent2`.`fields_cfg`, `Extent2`.`save_state_on_close`, `Extent2`.`group_store_items`, `Extent2`.`track_activity`, 
`Extent2`.`card_on_call`, `Extent2`.`inn`, `Extent2`.`inform_comment`, `Extent2`.`inform_status`, `Extent2`.`kkt`, `Extent2`.`pinpad`, `Extent2`.`advance_disable`, 
`Extent2`.`salary_disable`, `Extent2`.`notes`, `Extent3`.`id` AS `id2`, `Extent3`.`creator`, `Extent3`.`name` AS `name1`, `Extent3`.`surname` AS `surname1`, `Extent3`.`patronymic` AS 
`patronymic1`, `Extent3`.`agent_phone_mask`, `Extent3`.`agent2_phone_mask`, `Extent3`.`address` AS `address1`, `Extent3`.`post_index`, `Extent3`.`passport_num` AS `passport_num1`, 
`Extent3`.`passport_date` AS `passport_date1`, `Extent3`.`passport_organ` AS `passport_organ1`, `Extent3`.`state` AS `state2`, `Extent3`.`type` AS `type1`, `Extent3`.`birthday` AS 
`birthday1`, `Extent3`.`memorial`, `Extent3`.`notes` AS `notes1`, `Extent3`.`is_regular`, `Extent3`.`is_dealer`, `Extent3`.`balance_enable`, `Extent3`.`prefer_cashless`, 
`Extent3`.`take_long`, `Extent3`.`ignore_calls`, `Extent3`.`is_bad`, `Extent3`.`is_realizator`, `Extent3`.`is_agent`, `Extent3`.`visit_source`, `Extent3`.`photo_id`, `Extent3`.`INN` AS 
`inn1`, `Extent3`.`KPP`, `Extent3`.`OGRN`, `Extent3`.`web_password`, `Extent3`.`ur_name`, `Extent3`.`email` AS `email1`, `Extent3`.`icq`, `Extent3`.`skype`, `Extent3`.`viber`, 
`Extent3`.`telegram`, `Extent3`.`site`, `Extent3`.`whatsapp`, `Extent3`.`agent_name`, `Extent3`.`agent_surname`, `Extent3`.`agent_patronymic`, `Extent3`.`agent_phone`, 
`Extent3`.`agent_phone_clean`, `Extent3`.`agent2_name`, `Extent3`.`agent2_surname`, `Extent3`.`agent2_patronymic`, `Extent3`.`agent2_phone`, `Extent3`.`agent2_phone_clean`, 
`Extent3`.`created` AS `created1`, `Extent3`.`balance`, `Extent3`.`price_col`, `Extent3`.`repairs`, `Extent3`.`purchases`, `Extent3`.`token`, `Extent4`.`id` AS `id3`, `Extent4`.`num`, 
`Extent4`.`created` AS `created2`, `Extent4`.`user`, `Extent4`.`seller`, `Extent4`.`customer`, `Extent4`.`paid`, `Extent4`.`tax`, `Extent4`.`summ`, `Extent4`.`total`, `Extent4`.`state` AS 
`state3`, `Extent4`.`office` AS `office2`, `Extent4`.`type` AS `type2`, `Extent4`.`notes` AS `notes2`, `Extent5`.`id` AS `id4`, `Extent5`.`refill`, `Extent5`.`chip`, `Extent5`.`opc_drum`, 
`Extent5`.`c_blade`, `Extent5`.`card_id`, `Extent6`.`id` AS `id5`, `Extent6`.`name` AS `name2`, `Extent6`.`maker` AS `maker1`, `Extent6`.`full_weight`, `Extent6`.`toner_weight`, 
`Extent6`.`resource`, `Extent6`.`created` AS `created3`, `Extent6`.`user` AS `user1`, `Extent6`.`notes` AS `notes3`, `Extent6`.`photo` AS `photo1`, `Extent6`.`color` AS `color1`, 
`Extent6`.`archive`
FROM `workshop` AS `Extent1` LEFT OUTER JOIN `users` AS `Extent2` ON `Extent1`.`user_lock` = `Extent2`.`id` INNER JOIN `clients` AS `Extent3` ON `Extent1`.`client` = `Extent3`.`id` LEFT OUTER JOIN `invoice` AS `Extent4` ON `Extent1`.`invoice` = `Extent4`.`id` LEFT OUTER JOIN `c_workshop` AS `Extent5` ON `Extent1`.`cartridge` = `Extent5`.`id` LEFT OUTER JOIN `cartridge_cards` AS `Extent6` ON `Extent5`.`card_id` = `Extent6`.`id`
 WHERE `Extent1`.`id` = 24875 LIMIT 2) AS `Limit1` LEFT OUTER JOIN (SELECT
`Extent7`.`id`, `Extent7`.`field_id`, `Extent7`.`repair_id`, `Extent7`.`item_id`, `Extent7`.`value`, `Extent8`.`id` AS `ID1`, `Extent8`.`_f`, `Extent8`.`name`, `Extent8`.`type`, 
`Extent8`.`def_values`, `Extent8`.`required`, `Extent8`.`printable`, `Extent8`.`devices`, `Extent8`.`categories`, `Extent8`.`updated_at`, `Extent8`.`archive`
FROM `field_values` AS `Extent7` INNER JOIN `fields` AS `Extent8` ON `Extent7`.`field_id` = `Extent8`.`id`) AS `Join6` ON `Limit1`.`id` = `Join6`.`repair_id`) AS `Project2`
 ORDER BY 
`Project2`.`id` ASC, `Project2`.`id1` ASC, `Project2`.`id2` ASC, `Project2`.`id3` ASC, `Project2`.`id4` ASC, `Project2`.`id5` ASC, `Project2`.`C1` ASC;

SELECT
`GroupBy1`.`A1` AS `C1`
FROM (SELECT
SUM(CASE WHEN (`Project1`.`C1` IS NOT NULL) THEN (`Project1`.`summa`)  ELSE (0) END) AS `A1`
FROM (SELECT
1 AS `X`) AS `SingleRowTable1` LEFT OUTER JOIN (SELECT
`Extent1`.`summa`, 
1 AS `C1`
FROM `cash_orders` AS `Extent1`
 WHERE `Extent1`.`repair` = 24875) AS `Project1` ON 1 = 1) AS `GroupBy1`;

SELECT
`GroupBy1`.`A1` AS `C1`
FROM (SELECT
SUM(CASE WHEN (`Project1`.`C2` IS NOT NULL) THEN (`Project1`.`C1`)  ELSE (0) END) AS `A1`
FROM (SELECT
1 AS `X`) AS `SingleRowTable1` LEFT OUTER JOIN (SELECT
`Extent1`.`price` * (`Extent1`.`count`) AS `C1`, 
1 AS `C2`
FROM `works` AS `Extent1`
 WHERE `Extent1`.`repair` = 24875) AS `Project1` ON 1 = 1) AS `GroupBy1`;

SELECT
`GroupBy1`.`A1` AS `C1`
FROM (SELECT
SUM(CASE WHEN (`Project1`.`C2` IS NOT NULL) THEN (`Project1`.`C1`)  ELSE (0) END) AS `A1`
FROM (SELECT
1 AS `X`) AS `SingleRowTable1` LEFT OUTER JOIN (SELECT
(`Extent1`.`count`) * `Extent1`.`price` AS `C1`, 
1 AS `C2`
FROM `store_int_reserve` AS `Extent1`
 WHERE (`Extent1`.`repair_id` = 24875) AND ((2 = `Extent1`.`state`) OR (3 = `Extent1`.`state`))) AS `Project1` ON 1 = 1) AS `GroupBy1`;

SELECT
`GroupBy1`.`A1` AS `C1`
FROM (SELECT
SUM(CASE WHEN (`Project1`.`C1` IS NOT NULL) THEN (`Project1`.`summa`)  ELSE (0) END) AS `A1`
FROM (SELECT
1 AS `X`) AS `SingleRowTable1` LEFT OUTER JOIN (SELECT
`Extent1`.`summa`, 
1 AS `C1`
FROM `cash_orders` AS `Extent1`
 WHERE `Extent1`.`repair` = 24875) AS `Project1` ON 1 = 1) AS `GroupBy1`;

SELECT
`GroupBy1`.`A1` AS `C1`
FROM (SELECT
SUM(CASE WHEN (`Project1`.`C2` IS NOT NULL) THEN (`Project1`.`C1`)  ELSE (0) END) AS `A1`
FROM (SELECT
1 AS `X`) AS `SingleRowTable1` LEFT OUTER JOIN (SELECT
`Extent1`.`price` * (`Extent1`.`count`) AS `C1`, 
1 AS `C2`
FROM `works` AS `Extent1`
 WHERE `Extent1`.`repair` = 24875) AS `Project1` ON 1 = 1) AS `GroupBy1`;

SELECT
`GroupBy1`.`A1` AS `C1`
FROM (SELECT
SUM(CASE WHEN (`Project1`.`C2` IS NOT NULL) THEN (`Project1`.`C1`)  ELSE (0) END) AS `A1`
FROM (SELECT
1 AS `X`) AS `SingleRowTable1` LEFT OUTER JOIN (SELECT
(`Extent1`.`count`) * `Extent1`.`price` AS `C1`, 
1 AS `C2`
FROM `store_int_reserve` AS `Extent1`
 WHERE (`Extent1`.`repair_id` = 24875) AND ((2 = `Extent1`.`state`) OR (3 = `Extent1`.`state`))) AS `Project1` ON 1 = 1) AS `GroupBy1`;

SELECT
`Project2`.`id`, `Project2`.`creator`, `Project2`.`name`, `Project2`.`surname`, `Project2`.`patronymic`, `Project2`.`agent_phone_mask`, `Project2`.`agent2_phone_mask`, 
`Project2`.`address`, `Project2`.`post_index`, `Project2`.`passport_num`, `Project2`.`passport_date`, `Project2`.`passport_organ`, `Project2`.`state`, `Project2`.`type`, 
`Project2`.`birthday`, `Project2`.`memorial`, `Project2`.`notes`, `Project2`.`is_regular`, `Project2`.`is_dealer`, `Project2`.`balance_enable`, `Project2`.`prefer_cashless`, 
`Project2`.`take_long`, `Project2`.`ignore_calls`, `Project2`.`is_bad`, `Project2`.`is_realizator`, `Project2`.`is_agent`, `Project2`.`INN`, `Project2`.`KPP`, `Project2`.`OGRN`, 
`Project2`.`web_password`, `Project2`.`ur_name`, `Project2`.`email`, `Project2`.`icq`, `Project2`.`skype`, `Project2`.`viber`, `Project2`.`telegram`, `Project2`.`site`, 
`Project2`.`whatsapp`, `Project2`.`agent_name`, `Project2`.`agent_surname`, `Project2`.`agent_patronymic`, `Project2`.`agent_phone`, `Project2`.`agent2_name`, `Project2`.`agent2_surname`, 
`Project2`.`agent2_patronymic`, `Project2`.`agent2_phone`, `Project2`.`created`, `Project2`.`balance`, `Project2`.`price_col`, `Project2`.`agent_phone_clean`, 
`Project2`.`agent2_phone_clean`, `Project2`.`repairs`, `Project2`.`token`, `Project2`.`purchases`, `Project2`.`visit_source`, `Project2`.`photo_id`, `Project2`.`C1`, `Project2`.`id1`, 
`Project2`.`phone`, `Project2`.`phone_clean`, `Project2`.`mask`, `Project2`.`customer`, `Project2`.`type1`, `Project2`.`note`
FROM (SELECT
`Limit1`.`id`, `Limit1`.`creator`, `Limit1`.`name`, `Limit1`.`surname`, `Limit1`.`patronymic`, `Limit1`.`agent_phone_mask`, `Limit1`.`agent2_phone_mask`, `Limit1`.`address`, 
`Limit1`.`post_index`, `Limit1`.`passport_num`, `Limit1`.`passport_date`, `Limit1`.`passport_organ`, `Limit1`.`state`, `Limit1`.`type`, `Limit1`.`birthday`, `Limit1`.`memorial`, 
`Limit1`.`notes`, `Limit1`.`is_regular`, `Limit1`.`is_dealer`, `Limit1`.`balance_enable`, `Limit1`.`prefer_cashless`, `Limit1`.`take_long`, `Limit1`.`ignore_calls`, `Limit1`.`is_bad`, 
`Limit1`.`is_realizator`, `Limit1`.`is_agent`, `Limit1`.`visit_source`, `Limit1`.`photo_id`, `Limit1`.`INN`, `Limit1`.`KPP`, `Limit1`.`OGRN`, `Limit1`.`web_password`, `Limit1`.`ur_name`, 
`Limit1`.`email`, `Limit1`.`icq`, `Limit1`.`skype`, `Limit1`.`viber`, `Limit1`.`telegram`, `Limit1`.`site`, `Limit1`.`whatsapp`, `Limit1`.`agent_name`, `Limit1`.`agent_surname`, 
`Limit1`.`agent_patronymic`, `Limit1`.`agent_phone`, `Limit1`.`agent_phone_clean`, `Limit1`.`agent2_name`, `Limit1`.`agent2_surname`, `Limit1`.`agent2_patronymic`, `Limit1`.`agent2_phone`,
 `Limit1`.`agent2_phone_clean`, `Limit1`.`created`, `Limit1`.`balance`, `Limit1`.`price_col`, `Limit1`.`repairs`, `Limit1`.`purchases`, `Limit1`.`token`, `Extent2`.`id` AS `id1`, 
`Extent2`.`phone`, `Extent2`.`phone_clean`, `Extent2`.`mask`, `Extent2`.`customer`, `Extent2`.`type` AS `type1`, `Extent2`.`note`, CASE WHEN (`Extent2`.`id` IS NOT NULL) THEN (1)  ELSE 
(NULL) END AS `C1`
FROM (SELECT
`Extent1`.`id`, `Extent1`.`creator`, `Extent1`.`name`, `Extent1`.`surname`, `Extent1`.`patronymic`, `Extent1`.`agent_phone_mask`, `Extent1`.`agent2_phone_mask`, `Extent1`.`address`, 
`Extent1`.`post_index`, `Extent1`.`passport_num`, `Extent1`.`passport_date`, `Extent1`.`passport_organ`, `Extent1`.`state`, `Extent1`.`type`, `Extent1`.`birthday`, `Extent1`.`memorial`, 
`Extent1`.`notes`, `Extent1`.`is_regular`, `Extent1`.`is_dealer`, `Extent1`.`balance_enable`, `Extent1`.`prefer_cashless`, `Extent1`.`take_long`, `Extent1`.`ignore_calls`, 
`Extent1`.`is_bad`, `Extent1`.`is_realizator`, `Extent1`.`is_agent`, `Extent1`.`visit_source`, `Extent1`.`photo_id`, `Extent1`.`INN`, `Extent1`.`KPP`, `Extent1`.`OGRN`, 
`Extent1`.`web_password`, `Extent1`.`ur_name`, `Extent1`.`email`, `Extent1`.`icq`, `Extent1`.`skype`, `Extent1`.`viber`, `Extent1`.`telegram`, `Extent1`.`site`, `Extent1`.`whatsapp`, 
`Extent1`.`agent_name`, `Extent1`.`agent_surname`, `Extent1`.`agent_patronymic`, `Extent1`.`agent_phone`, `Extent1`.`agent_phone_clean`, `Extent1`.`agent2_name`, 
`Extent1`.`agent2_surname`, `Extent1`.`agent2_patronymic`, `Extent1`.`agent2_phone`, `Extent1`.`agent2_phone_clean`, `Extent1`.`created`, `Extent1`.`balance`, `Extent1`.`price_col`, 
`Extent1`.`repairs`, `Extent1`.`purchases`, `Extent1`.`token`
FROM `clients` AS `Extent1`
 WHERE `Extent1`.`id` = 7838 LIMIT 1) AS `Limit1` LEFT OUTER JOIN `tel` AS `Extent2` ON `Limit1`.`id` = `Extent2`.`customer`) AS `Project2`
 ORDER BY 
`Project2`.`id` ASC, `Project2`.`C1` ASC;

SELECT
`Extent1`.`id`, 
`Extent1`.`name`, 
`Extent1`.`position`, 
`Extent1`.`enabled`, 
`Extent1`.`fire_on`
FROM `visit_sources` AS `Extent1`
 WHERE `Extent1`.`id` = 12;

SELECT UTC_TIMESTAMP() FROM DUAL;

SELECT
`Limit2`.`machine_name`
FROM (SELECT
`Limit1`.`machine_name`
FROM (SELECT
1 AS `X`) AS `SingleRowTable1` LEFT OUTER JOIN (SELECT
`Project1`.`id`, 
`Project1`.`machine_name`
FROM `users_activity` AS `Project1`
 WHERE (`Project1`.`user_id` = 32) AND ('Выполнен вход в систему' = `Project1`.`notes`)
 ORDER BY 
`Project1`.`id` DESC LIMIT 1) AS `Limit1` ON 1 = 1 LIMIT 1) AS `Limit2`;

SELECT
`GroupBy1`.`A1` AS `C1`
FROM (SELECT
SUM(CASE WHEN (`Project1`.`C2` IS NOT NULL) THEN (`Project1`.`C1`)  ELSE (0) END) AS `A1`
FROM (SELECT
1 AS `X`) AS `SingleRowTable1` LEFT OUTER JOIN (SELECT
`Extent1`.`price` * (`Extent1`.`count`) AS `C1`, 
1 AS `C2`
FROM `works` AS `Extent1`
 WHERE `Extent1`.`repair` = 24875) AS `Project1` ON 1 = 1) AS `GroupBy1`;

SELECT
`GroupBy1`.`A1` AS `C1`
FROM (SELECT
SUM(CASE WHEN (`Project1`.`C2` IS NOT NULL) THEN (`Project1`.`C1`)  ELSE (0) END) AS `A1`
FROM (SELECT
1 AS `X`) AS `SingleRowTable1` LEFT OUTER JOIN (SELECT
(`Extent1`.`count`) * `Extent1`.`price` AS `C1`, 
1 AS `C2`
FROM `store_int_reserve` AS `Extent1`
 WHERE (`Extent1`.`repair_id` = 24875) AND ((2 = `Extent1`.`state`) OR (3 = `Extent1`.`state`))) AS `Project1` ON 1 = 1) AS `GroupBy1`;

SELECT
`Extent1`.`id`, `Extent1`.`item_id`, `Extent1`.`count`, `Extent1`.`created`, `Extent1`.`from_user`, `Extent1`.`to_user`, `Extent1`.`notes`, `Extent1`.`state`, `Extent1`.`repair_id`, 
`Extent1`.`price`, `Extent1`.`sn`, `Extent1`.`work_id`, `Extent1`.`warranty`, `Extent1`.`name`, `Extent1`.`r_lock`, `Extent2`.`id` AS `id1`, `Extent2`.`articul`, `Extent2`.`dealer`, 
`Extent2`.`is_realization`, `Extent2`.`dealer_lock`, `Extent2`.`name` AS `name1`, `Extent2`.`state` AS `state1`, `Extent2`.`category`, `Extent2`.`store`, `Extent2`.`created` AS `created1`,
 `Extent2`.`count` AS `count1`, `Extent2`.`reserved`, `Extent2`.`box`, `Extent2`.`box_name`, `Extent2`.`price_option`, `Extent2`.`currency_rate`, `Extent2`.`in_price`, `Extent2`.`price` AS
 `price1`, `Extent2`.`price2`, `Extent2`.`price3`, `Extent2`.`price4`, `Extent2`.`price5`, `Extent2`.`document`, `Extent2`.`shop_title`, `Extent2`.`shop_description`, `Extent2`.`SN` AS 
`sn1`, `Extent2`.`PN`, `Extent2`.`description`, `Extent2`.`shop_enable`, `Extent2`.`int_barcode`, `Extent2`.`ext_barcode`, `Extent2`.`in_summ`, `Extent2`.`notes` AS `notes1`, 
`Extent2`.`img1`, `Extent2`.`img2`, `Extent2`.`img3`, `Extent2`.`img4`, `Extent2`.`img5`, `Extent2`.`minimum_in_stock`, `Extent2`.`sold`, `Extent2`.`return_percent`, `Extent2`.`warranty` 
AS `warranty1`, `Extent2`.`warranty_dealer`, `Extent2`.`in_count`, `Extent2`.`not_for_sale`, `Extent2`.`part_request`, `Extent2`.`st_state`, `Extent2`.`st_notes`, `Extent2`.`units`, 
`Extent2`.`ge_highlight`, `Extent2`.`Hidden`, `Extent2`.`updated`
FROM `store_int_reserve` AS `Extent1` INNER JOIN `store_items` AS `Extent2` ON `Extent1`.`item_id` = `Extent2`.`id`
 WHERE (`Extent1`.`repair_id` = 24875) AND ((2 = `Extent1`.`state`) OR (3 = `Extent1`.`state`));

SELECT
`Extent1`.`id`, `Extent1`.`articul`, `Extent1`.`dealer`, `Extent1`.`is_realization`, `Extent1`.`dealer_lock`, `Extent1`.`name`, `Extent1`.`state`, `Extent1`.`category`, `Extent1`.`store`, 
`Extent1`.`created`, `Extent1`.`count`, `Extent1`.`reserved`, `Extent1`.`box`, `Extent1`.`box_name`, `Extent1`.`price_option`, `Extent1`.`currency_rate`, `Extent1`.`in_price`, 
`Extent1`.`price`, `Extent1`.`price2`, `Extent1`.`price3`, `Extent1`.`price4`, `Extent1`.`price5`, `Extent1`.`document`, `Extent1`.`shop_title`, `Extent1`.`shop_description`, 
`Extent1`.`SN`, `Extent1`.`PN`, `Extent1`.`description`, `Extent1`.`shop_enable`, `Extent1`.`int_barcode`, `Extent1`.`ext_barcode`, `Extent1`.`in_summ`, `Extent1`.`notes`, 
`Extent1`.`img1`, `Extent1`.`img2`, `Extent1`.`img3`, `Extent1`.`img4`, `Extent1`.`img5`, `Extent1`.`minimum_in_stock`, `Extent1`.`sold`, `Extent1`.`return_percent`, `Extent1`.`warranty`, 
`Extent1`.`warranty_dealer`, `Extent1`.`in_count`, `Extent1`.`not_for_sale`, `Extent1`.`part_request`, `Extent1`.`st_state`, `Extent1`.`st_notes`, `Extent1`.`units`, 
`Extent1`.`ge_highlight`, `Extent1`.`Hidden`, `Extent1`.`updated`
FROM `store_items` AS `Extent1`
 WHERE `Extent1`.`id` = 23901 LIMIT 1;

SELECT
`Extent1`.`id`, `Extent1`.`item_id`, `Extent1`.`count`, `Extent1`.`created`, `Extent1`.`from_user`, `Extent1`.`to_user`, `Extent1`.`notes`, `Extent1`.`state`, `Extent1`.`repair_id`, 
`Extent1`.`price`, `Extent1`.`sn`, `Extent1`.`work_id`, `Extent1`.`warranty`, `Extent1`.`name`, `Extent1`.`r_lock`
FROM `store_int_reserve` AS `Extent1`
 WHERE `Extent1`.`id` = 19056 LIMIT 2;

BEGIN;

UPDATE `store_int_reserve` SET `state`=3 WHERE `id` = 19056;

COMMIT;

BEGIN;

UPDATE `store_items` SET `count`=23, `reserved`=1, `sold`=86 WHERE `id` = 23901;

COMMIT;

SELECT
`Extent1`.`id`, `Extent1`.`articul`, `Extent1`.`dealer`, `Extent1`.`is_realization`, `Extent1`.`dealer_lock`, `Extent1`.`name`, `Extent1`.`state`, `Extent1`.`category`, `Extent1`.`store`, 
`Extent1`.`created`, `Extent1`.`count`, `Extent1`.`reserved`, `Extent1`.`box`, `Extent1`.`box_name`, `Extent1`.`price_option`, `Extent1`.`currency_rate`, `Extent1`.`in_price`, 
`Extent1`.`price`, `Extent1`.`price2`, `Extent1`.`price3`, `Extent1`.`price4`, `Extent1`.`price5`, `Extent1`.`document`, `Extent1`.`shop_title`, `Extent1`.`shop_description`, 
`Extent1`.`SN`, `Extent1`.`PN`, `Extent1`.`description`, `Extent1`.`shop_enable`, `Extent1`.`int_barcode`, `Extent1`.`ext_barcode`, `Extent1`.`in_summ`, `Extent1`.`notes`, 
`Extent1`.`img1`, `Extent1`.`img2`, `Extent1`.`img3`, `Extent1`.`img4`, `Extent1`.`img5`, `Extent1`.`minimum_in_stock`, `Extent1`.`sold`, `Extent1`.`return_percent`, `Extent1`.`warranty`, 
`Extent1`.`warranty_dealer`, `Extent1`.`in_count`, `Extent1`.`not_for_sale`, `Extent1`.`part_request`, `Extent1`.`st_state`, `Extent1`.`st_notes`, `Extent1`.`units`, 
`Extent1`.`ge_highlight`, `Extent1`.`Hidden`, `Extent1`.`updated`
FROM `store_items` AS `Extent1`
 WHERE `Extent1`.`id` = 27534 LIMIT 1;

SELECT
`Extent1`.`id`, `Extent1`.`item_id`, `Extent1`.`count`, `Extent1`.`created`, `Extent1`.`from_user`, `Extent1`.`to_user`, `Extent1`.`notes`, `Extent1`.`state`, `Extent1`.`repair_id`, 
`Extent1`.`price`, `Extent1`.`sn`, `Extent1`.`work_id`, `Extent1`.`warranty`, `Extent1`.`name`, `Extent1`.`r_lock`
FROM `store_int_reserve` AS `Extent1`
 WHERE `Extent1`.`id` = 19071 LIMIT 2;

BEGIN;

UPDATE `store_int_reserve` SET `state`=3 WHERE `id` = 19071;

COMMIT;

BEGIN;

UPDATE `store_items` SET `count`=0, `reserved`=0, `sold`=1 WHERE `id` = 27534;

COMMIT;

BEGIN;

INSERT INTO `logs`(
`group`, `type`, `arh`, `user`, `created`, `values`, `values_after`, `office`, `client`, `repair`, `item`, `document`, `cash_order`, `part_request`, `notes`) VALUES (
NULL, 6, 0, 32, '2022-03-24 23:48:38.786111', NULL, NULL, 1, 7838, 24875, 23901, NULL, NULL, NULL, 'Товар установленный в ремонт 024875 продан. Ремонт выдан');
SELECT
`id`
FROM `logs`
 WHERE  row_count() > 0 AND `id`=last_insert_id();

INSERT INTO `logs`(
`group`, `type`, `arh`, `user`, `created`, `values`, `values_after`, `office`, `client`, `repair`, `item`, `document`, `cash_order`, `part_request`, `notes`) VALUES (
NULL, 6, 0, 32, '2022-03-24 23:48:38.795132', NULL, NULL, 1, 7838, 24875, 27534, NULL, NULL, NULL, 'Товар установленный в ремонт 024875 продан. Ремонт выдан');
SELECT
`id`
FROM `logs`
 WHERE  row_count() > 0 AND `id`=last_insert_id();

COMMIT;

SELECT
`Extent1`.`id`, `Extent1`.`client`, `Extent1`.`type`, `Extent1`.`maker`, `Extent1`.`model`, `Extent1`.`serial_number`, `Extent1`.`company`, `Extent1`.`office`, `Extent1`.`start_office`, 
`Extent1`.`manager`, `Extent1`.`current_manager`, `Extent1`.`master`, `Extent1`.`diagnostic_result`, `Extent1`.`in_date`, `Extent1`.`out_date`, `Extent1`.`state`, `Extent1`.`user_lock`, 
`Extent1`.`lock_datetime`, `Extent1`.`express_repair`, `Extent1`.`is_warranty`, `Extent1`.`is_repeat`, `Extent1`.`can_format`, `Extent1`.`print_check`, `Extent1`.`box`, 
`Extent1`.`warranty_label`, `Extent1`.`ext_notes`, `Extent1`.`is_prepaid`, `Extent1`.`prepaid_type`, `Extent1`.`prepaid_summ`, `Extent1`.`prepaid_order`, `Extent1`.`is_pre_agreed`, 
`Extent1`.`pre_agreed_amount`, `Extent1`.`repair_cost`, `Extent1`.`fault`, `Extent1`.`complect`, `Extent1`.`look`, `Extent1`.`thirs_party_sc`, `Extent1`.`last_save`, 
`Extent1`.`last_status_changed`, `Extent1`.`warranty_days`, `Extent1`.`barcode`, `Extent1`.`new_state`, `Extent1`.`is_card_payment`, `Extent1`.`informed_status`, `Extent1`.`parts_cost`, 
`Extent1`.`is_debt`, `Extent1`.`image_ids`, `Extent1`.`color`, `Extent1`.`order_moving`, `Extent1`.`payment_system`, `Extent1`.`early`, `Extent1`.`real_repair_cost`, 
`Extent1`.`issued_msg`, `Extent1`.`reject_reason`, `Extent1`.`ext_early`, `Extent1`.`sms_inform`, `Extent1`.`invoice`, `Extent1`.`cartridge`, `Extent1`.`termsControl`, 
`Extent1`.`vendor_id`, `Extent1`.`quick_repair`, `Extent1`.`Hidden`, `Extent1`.`Title`
FROM `workshop` AS `Extent1`
 WHERE `Extent1`.`id` = 24875;

SELECT UTC_TIMESTAMP() FROM DUAL;

SELECT
`Extent1`.`id`, `Extent1`.`name`, `Extent1`.`places`, `Extent1`.`store_id`, `Extent1`.`non_items`, `Extent1`.`color`
FROM `boxes` AS `Extent1`
 WHERE `Extent1`.`id` = 1035;

BEGIN;

UPDATE `workshop` SET `out_date`='2022-03-24 23:48:38', `state`=8, `box`=NULL, `last_status_changed`='2022-03-24 23:48:38' WHERE `id` = 24875;

INSERT INTO `workshop_issued`(
`repair_id`, 
`employee_id`, 
`created_at`) VALUES (
24875, 
32, 
'2022-03-24 23:48:38');

SELECT
`id`
FROM `workshop_issued`
 WHERE  row_count() > 0 AND `id`=last_insert_id();

COMMIT;

BEGIN;

INSERT INTO `logs`(
`group`, `type`, `arh`, `user`, `created`, `values`, `values_after`, `office`, `client`, `repair`, `item`, `document`, `cash_order`, `part_request`, `notes`) VALUES (
NULL, 3, 0, 32, '2022-03-24 23:48:38.803150', NULL, NULL, 1, NULL, 24875, NULL, NULL, NULL, NULL, 'Заказаз-наряд: 024875, изъят из ячейки RPR/B2');

SELECT
`id`
FROM `logs`
 WHERE  row_count() > 0 AND `id`=last_insert_id();

INSERT INTO `logs`(
`group`, `type`, `arh`, `user`, `created`, `values`, `values_after`, `office`, `client`, `repair`, `item`, `document`, `cash_order`, `part_request`, `notes`) VALUES (
NULL, 3, 0, 32, '2022-03-24 23:48:38.817181', NULL, NULL, 1, NULL, 24875, NULL, NULL, NULL, NULL, 'Заказаз-наряд: 024875, выдан клиенту. Сумма: 3 200,00 грн., оплата: Наличные');

SELECT
`id`
FROM `logs`
 WHERE  row_count() > 0 AND `id`=last_insert_id();

COMMIT;

SELECT UTC_TIMESTAMP() FROM DUAL;

BEGIN;

INSERT INTO `repair_status_logs`(
`created_at`, `repair_id`, `status_id`, `user_id`, `manager_id`, `master_id`) VALUES (
'2022-03-24 23:48:38', 24875, 6, 32, 34, 40);

SELECT
`id`
FROM `repair_status_logs`
 WHERE  row_count() > 0 AND `id`=last_insert_id();

COMMIT;

SELECT
`GroupBy1`.`A1` AS `C1`
FROM (SELECT
SUM(CASE WHEN (`Project1`.`C2` IS NOT NULL) THEN (`Project1`.`C1`)  ELSE (0) END) AS `A1`
FROM (SELECT
1 AS `X`) AS `SingleRowTable1` LEFT OUTER JOIN (SELECT
`Extent1`.`price` * (`Extent1`.`count`) AS `C1`, 
1 AS `C2`
FROM `works` AS `Extent1`
 WHERE `Extent1`.`repair` = 24875) AS `Project1` ON 1 = 1) AS `GroupBy1`;

SELECT
`GroupBy1`.`A1` AS `C1`
FROM (SELECT
SUM(CASE WHEN (`Project1`.`C2` IS NOT NULL) THEN (`Project1`.`C1`)  ELSE (0) END) AS `A1`
FROM (SELECT
1 AS `X`) AS `SingleRowTable1` LEFT OUTER JOIN (SELECT
(`Extent1`.`count`) * `Extent1`.`price` AS `C1`, 
1 AS `C2`
FROM `store_int_reserve` AS `Extent1`
 WHERE (`Extent1`.`repair_id` = 24875) AND ((2 = `Extent1`.`state`) OR (3 = `Extent1`.`state`))) AS `Project1` ON 1 = 1) AS `GroupBy1`;

SELECT
`GroupBy1`.`A1` AS `C1`
FROM (SELECT
SUM(CASE WHEN (`Project1`.`C1` IS NOT NULL) THEN (`Project1`.`summa`)  ELSE (0) END) AS `A1`
FROM (SELECT
1 AS `X`) AS `SingleRowTable1` LEFT OUTER JOIN (SELECT
`Extent1`.`summa`, 
1 AS `C1`
FROM `cash_orders` AS `Extent1`
 WHERE `Extent1`.`repair` = 24875) AS `Project1` ON 1 = 1) AS `GroupBy1`;

SELECT
`Project2`.`id`, `Project2`.`creator`, `Project2`.`name`, `Project2`.`surname`, `Project2`.`patronymic`, `Project2`.`agent_phone_mask`, `Project2`.`agent2_phone_mask`, 
`Project2`.`address`, `Project2`.`post_index`, `Project2`.`passport_num`, `Project2`.`passport_date`, `Project2`.`passport_organ`, `Project2`.`state`, `Project2`.`type`, 
`Project2`.`birthday`, `Project2`.`memorial`, `Project2`.`notes`, `Project2`.`is_regular`, `Project2`.`is_dealer`, `Project2`.`balance_enable`, `Project2`.`prefer_cashless`, 
`Project2`.`take_long`, `Project2`.`ignore_calls`, `Project2`.`is_bad`, `Project2`.`is_realizator`, `Project2`.`is_agent`, `Project2`.`INN`, `Project2`.`KPP`, `Project2`.`OGRN`, 
`Project2`.`web_password`, `Project2`.`ur_name`, `Project2`.`email`, `Project2`.`icq`, `Project2`.`skype`, `Project2`.`viber`, `Project2`.`telegram`, `Project2`.`site`, 
`Project2`.`whatsapp`, `Project2`.`agent_name`, `Project2`.`agent_surname`, `Project2`.`agent_patronymic`, `Project2`.`agent_phone`, `Project2`.`agent2_name`, `Project2`.`agent2_surname`, 
`Project2`.`agent2_patronymic`, `Project2`.`agent2_phone`, `Project2`.`created`, `Project2`.`balance`, `Project2`.`price_col`, `Project2`.`agent_phone_clean`, 
`Project2`.`agent2_phone_clean`, `Project2`.`repairs`, `Project2`.`token`, `Project2`.`purchases`, `Project2`.`visit_source`, `Project2`.`photo_id`, `Project2`.`C1`, `Project2`.`id1`, 
`Project2`.`phone`, `Project2`.`phone_clean`, `Project2`.`mask`, `Project2`.`customer`, `Project2`.`type1`, `Project2`.`note`
FROM (SELECT
`Limit1`.`id`, `Limit1`.`creator`, `Limit1`.`name`, `Limit1`.`surname`, `Limit1`.`patronymic`, `Limit1`.`agent_phone_mask`, `Limit1`.`agent2_phone_mask`, `Limit1`.`address`, 
`Limit1`.`post_index`, `Limit1`.`passport_num`, `Limit1`.`passport_date`, `Limit1`.`passport_organ`, `Limit1`.`state`, `Limit1`.`type`, `Limit1`.`birthday`, `Limit1`.`memorial`, 
`Limit1`.`notes`, `Limit1`.`is_regular`, `Limit1`.`is_dealer`, `Limit1`.`balance_enable`, `Limit1`.`prefer_cashless`, `Limit1`.`take_long`, `Limit1`.`ignore_calls`, `Limit1`.`is_bad`, 
`Limit1`.`is_realizator`, `Limit1`.`is_agent`, `Limit1`.`visit_source`, `Limit1`.`photo_id`, `Limit1`.`INN`, `Limit1`.`KPP`, `Limit1`.`OGRN`, `Limit1`.`web_password`, `Limit1`.`ur_name`, 
`Limit1`.`email`, `Limit1`.`icq`, `Limit1`.`skype`, `Limit1`.`viber`, `Limit1`.`telegram`, `Limit1`.`site`, `Limit1`.`whatsapp`, `Limit1`.`agent_name`, `Limit1`.`agent_surname`, 
`Limit1`.`agent_patronymic`, `Limit1`.`agent_phone`, `Limit1`.`agent_phone_clean`, `Limit1`.`agent2_name`, `Limit1`.`agent2_surname`, `Limit1`.`agent2_patronymic`, `Limit1`.`agent2_phone`,
 `Limit1`.`agent2_phone_clean`, `Limit1`.`created`, `Limit1`.`balance`, `Limit1`.`price_col`, `Limit1`.`repairs`, `Limit1`.`purchases`, `Limit1`.`token`, `Extent2`.`id` AS `id1`, 
`Extent2`.`phone`, `Extent2`.`phone_clean`, `Extent2`.`mask`, `Extent2`.`customer`, `Extent2`.`type` AS `type1`, `Extent2`.`note`, CASE WHEN (`Extent2`.`id` IS NOT NULL) THEN (1)  ELSE 
(NULL) END AS `C1`
FROM (SELECT
`Extent1`.`id`, `Extent1`.`creator`, `Extent1`.`name`, `Extent1`.`surname`, `Extent1`.`patronymic`, `Extent1`.`agent_phone_mask`, `Extent1`.`agent2_phone_mask`, `Extent1`.`address`, 
`Extent1`.`post_index`, `Extent1`.`passport_num`, `Extent1`.`passport_date`, `Extent1`.`passport_organ`, `Extent1`.`state`, `Extent1`.`type`, `Extent1`.`birthday`, `Extent1`.`memorial`, 
`Extent1`.`notes`, `Extent1`.`is_regular`, `Extent1`.`is_dealer`, `Extent1`.`balance_enable`, `Extent1`.`prefer_cashless`, `Extent1`.`take_long`, `Extent1`.`ignore_calls`, 
`Extent1`.`is_bad`, `Extent1`.`is_realizator`, `Extent1`.`is_agent`, `Extent1`.`visit_source`, `Extent1`.`photo_id`, `Extent1`.`INN`, `Extent1`.`KPP`, `Extent1`.`OGRN`, 
`Extent1`.`web_password`, `Extent1`.`ur_name`, `Extent1`.`email`, `Extent1`.`icq`, `Extent1`.`skype`, `Extent1`.`viber`, `Extent1`.`telegram`, `Extent1`.`site`, `Extent1`.`whatsapp`, 
`Extent1`.`agent_name`, `Extent1`.`agent_surname`, `Extent1`.`agent_patronymic`, `Extent1`.`agent_phone`, `Extent1`.`agent_phone_clean`, `Extent1`.`agent2_name`, 
`Extent1`.`agent2_surname`, `Extent1`.`agent2_patronymic`, `Extent1`.`agent2_phone`, `Extent1`.`agent2_phone_clean`, `Extent1`.`created`, `Extent1`.`balance`, `Extent1`.`price_col`, 
`Extent1`.`repairs`, `Extent1`.`purchases`, `Extent1`.`token`
FROM `clients` AS `Extent1`
 WHERE `Extent1`.`id` = 7838 LIMIT 1) AS `Limit1` LEFT OUTER JOIN `tel` AS `Extent2` ON `Limit1`.`id` = `Extent2`.`customer`) AS `Project2`
 ORDER BY 
`Project2`.`id` ASC, 
`Project2`.`C1` ASC;

SELECT
`Extent1`.`id`, `Extent1`.`name`, `Extent1`.`position`, `Extent1`.`enabled`, `Extent1`.`fire_on`FROM `visit_sources` AS `Extent1`
 WHERE `Extent1`.`id` = 12;

SELECT UTC_TIMESTAMP() FROM DUAL;

BEGIN;

INSERT INTO `cash_orders`(
`created`, `type`, `summa`, `summa_str`, `invoice`, `client`, `to_user`, `user`, `company`, `office`, `notes`, `repair`, `document`, `img`, `payment_system`, `card_fee`, `is_backdate`, 
`card_info`, `customer_email`, `fdn`, `payment_item_sign`) VALUES (
'2022-03-24 23:48:38', 15, 3200.0000, 'три тысячи двести гривен 00 коп.', NULL, 7838, NULL, 32, 1, 1, 'Поступление денег в размере 3 200,00 грн. в счёт выполненого ремонта №024875', 24875,
 NULL, NULL, 0, 0, 0, NULL, NULL, NULL, NULL);

 SELECT
`id`
FROM `cash_orders`
 WHERE  row_count() > 0 AND `id`=last_insert_id();

COMMIT;

SELECT
`Extent1`.`id`, `Extent1`.`client`, `Extent1`.`type`, `Extent1`.`maker`, `Extent1`.`model`, `Extent1`.`serial_number`, `Extent1`.`company`, `Extent1`.`office`, `Extent1`.`start_office`, 
`Extent1`.`manager`, `Extent1`.`current_manager`, `Extent1`.`master`, `Extent1`.`diagnostic_result`, `Extent1`.`in_date`, `Extent1`.`out_date`, `Extent1`.`state`, `Extent1`.`user_lock`, 
`Extent1`.`lock_datetime`, `Extent1`.`express_repair`, `Extent1`.`is_warranty`, `Extent1`.`is_repeat`, `Extent1`.`can_format`, `Extent1`.`print_check`, `Extent1`.`box`, 
`Extent1`.`warranty_label`, `Extent1`.`ext_notes`, `Extent1`.`is_prepaid`, `Extent1`.`prepaid_type`, `Extent1`.`prepaid_summ`, `Extent1`.`prepaid_order`, `Extent1`.`is_pre_agreed`, 
`Extent1`.`pre_agreed_amount`, `Extent1`.`repair_cost`, `Extent1`.`fault`, `Extent1`.`complect`, `Extent1`.`look`, `Extent1`.`thirs_party_sc`, `Extent1`.`last_save`, 
`Extent1`.`last_status_changed`, `Extent1`.`warranty_days`, `Extent1`.`barcode`, `Extent1`.`new_state`, `Extent1`.`is_card_payment`, `Extent1`.`informed_status`, `Extent1`.`parts_cost`, 
`Extent1`.`is_debt`, `Extent1`.`image_ids`, `Extent1`.`color`, `Extent1`.`order_moving`, `Extent1`.`payment_system`, `Extent1`.`early`, `Extent1`.`real_repair_cost`, 
`Extent1`.`issued_msg`, `Extent1`.`reject_reason`, `Extent1`.`ext_early`, `Extent1`.`sms_inform`, `Extent1`.`invoice`, `Extent1`.`cartridge`, `Extent1`.`termsControl`, 
`Extent1`.`vendor_id`, `Extent1`.`quick_repair`, `Extent1`.`Hidden`, `Extent1`.`Title`
FROM `workshop` AS `Extent1`
 WHERE `Extent1`.`id` = 24875 LIMIT 2;

SELECT
`Project2`.`id`, `Project2`.`client`, `Project2`.`type`, `Project2`.`maker`, `Project2`.`model`, `Project2`.`serial_number`, `Project2`.`company`, `Project2`.`office`, 
`Project2`.`start_office`, `Project2`.`manager`, `Project2`.`current_manager`, `Project2`.`master`, `Project2`.`diagnostic_result`, `Project2`.`in_date`, `Project2`.`out_date`, 
`Project2`.`state`, `Project2`.`user_lock`, `Project2`.`lock_datetime`, `Project2`.`express_repair`, `Project2`.`is_warranty`, `Project2`.`is_repeat`, `Project2`.`can_format`, 
`Project2`.`print_check`, `Project2`.`box`, `Project2`.`warranty_label`, `Project2`.`ext_notes`, `Project2`.`is_prepaid`, `Project2`.`prepaid_type`, `Project2`.`prepaid_summ`, 
`Project2`.`prepaid_order`, `Project2`.`is_pre_agreed`, `Project2`.`pre_agreed_amount`, `Project2`.`repair_cost`, `Project2`.`fault`, `Project2`.`complect`, `Project2`.`look`, 
`Project2`.`thirs_party_sc`, `Project2`.`last_save`, `Project2`.`last_status_changed`, `Project2`.`warranty_days`, `Project2`.`barcode`, `Project2`.`new_state`, 
`Project2`.`is_card_payment`, `Project2`.`informed_status`, `Project2`.`parts_cost`, `Project2`.`is_debt`, `Project2`.`image_ids`, `Project2`.`color`, `Project2`.`order_moving`, 
`Project2`.`payment_system`, `Project2`.`early`, `Project2`.`real_repair_cost`, `Project2`.`issued_msg`, `Project2`.`reject_reason`, `Project2`.`ext_early`, `Project2`.`sms_inform`, 
`Project2`.`invoice`, `Project2`.`cartridge`, `Project2`.`termsControl`, `Project2`.`vendor_id`, `Project2`.`quick_repair`, `Project2`.`Hidden`, `Project2`.`Title`, `Project2`.`id1`, 
`Project2`.`sip_user_id`, `Project2`.`username`, `Project2`.`name`, `Project2`.`surname`, `Project2`.`patronymic`, `Project2`.`phone`, `Project2`.`phone2`, `Project2`.`phone_mask`, 
`Project2`.`phone2_mask`, `Project2`.`address`, `Project2`.`passport_num`, `Project2`.`passport_date`, `Project2`.`passport_organ`, `Project2`.`state1`, `Project2`.`created`, 
`Project2`.`office1`, `Project2`.`birthday`, `Project2`.`last_login`, `Project2`.`last_activity`, `Project2`.`email`, `Project2`.`sex`, `Project2`.`photo`, `Project2`.`salary_rate`, 
`Project2`.`pay_day`, `Project2`.`pay_day_off`, `Project2`.`pay_repair`, `Project2`.`pay_sale`, `Project2`.`pay_repair_q_sale`, `Project2`.`pay_cartridge_refill`, 
`Project2`.`color_label_ws`, `Project2`.`workspace_mode`, `Project2`.`preview_before_print`, `Project2`.`new_rep_doc_copies`, `Project2`.`auto_refresh_workspace`, 
`Project2`.`refresh_time`, `Project2`.`xls_c1`, `Project2`.`xls_c2`, `Project2`.`xls_c3`, `Project2`.`xls_c4`, `Project2`.`xls_c5`, `Project2`.`xls_c6`, `Project2`.`xls_c7`, 
`Project2`.`xls_c8`, `Project2`.`xls_c9`, `Project2`.`xls_c10`, `Project2`.`xls_c11`, `Project2`.`xls_c12`, `Project2`.`xls_c13`, `Project2`.`xls_c14`, `Project2`.`xls_c15`, 
`Project2`.`display_out`, `Project2`.`display_complete`, `Project2`.`is_bot`, `Project2`.`new_on_top`, `Project2`.`issued_color`, `Project2`.`fields_cfg`, `Project2`.`kkm_pass`, 
`Project2`.`prefer_regular`, `Project2`.`fontsize`, `Project2`.`rowheight`, `Project2`.`animation`, `Project2`.`pay_device_in`, `Project2`.`pay_device_out`, 
`Project2`.`pay_4_sale_in_repair`, `Project2`.`row_color`, `Project2`.`save_state_on_close`, `Project2`.`group_store_items`, `Project2`.`track_activity`, `Project2`.`inn`, 
`Project2`.`inform_comment`, `Project2`.`inform_status`, `Project2`.`kkt`, `Project2`.`pinpad`, `Project2`.`def_office`, `Project2`.`def_store`, `Project2`.`def_item_state`, 
`Project2`.`def_employee`, `Project2`.`def_status`, `Project2`.`def_ws_filter`, `Project2`.`card_on_call`, `Project2`.`ge_highlight_color`, `Project2`.`pay_repair_quick`, 
`Project2`.`advance_disable`, `Project2`.`salary_disable`, `Project2`.`notes`, `Project2`.`id2`, `Project2`.`creator`, `Project2`.`name1`, `Project2`.`surname1`, `Project2`.`patronymic1`, 
`Project2`.`agent_phone_mask`, `Project2`.`agent2_phone_mask`, `Project2`.`address1`, `Project2`.`post_index`, `Project2`.`passport_num1`, `Project2`.`passport_date1`, 
`Project2`.`passport_organ1`, `Project2`.`state2`, `Project2`.`type1`, `Project2`.`birthday1`, `Project2`.`memorial`, `Project2`.`notes1`, `Project2`.`is_regular`, `Project2`.`is_dealer`, 
`Project2`.`balance_enable`, `Project2`.`prefer_cashless`, `Project2`.`take_long`, `Project2`.`ignore_calls`, `Project2`.`is_bad`, `Project2`.`is_realizator`, `Project2`.`is_agent`, 
`Project2`.`inn1`, `Project2`.`KPP`, `Project2`.`OGRN`, `Project2`.`web_password`, `Project2`.`ur_name`, `Project2`.`email1`, `Project2`.`icq`, `Project2`.`skype`, `Project2`.`viber`, 
`Project2`.`telegram`, `Project2`.`site`, `Project2`.`whatsapp`, `Project2`.`agent_name`, `Project2`.`agent_surname`, `Project2`.`agent_patronymic`, `Project2`.`agent_phone`, 
`Project2`.`agent2_name`, `Project2`.`agent2_surname`, `Project2`.`agent2_patronymic`, `Project2`.`agent2_phone`, `Project2`.`created1`, `Project2`.`balance`, `Project2`.`price_col`, 
`Project2`.`agent_phone_clean`, `Project2`.`agent2_phone_clean`, `Project2`.`repairs`, `Project2`.`token`, `Project2`.`purchases`, `Project2`.`visit_source`, `Project2`.`photo_id`, 
`Project2`.`id3`, `Project2`.`num`, `Project2`.`created2`, `Project2`.`user`, `Project2`.`seller`, `Project2`.`customer`, `Project2`.`paid`, `Project2`.`tax`, `Project2`.`summ`, 
`Project2`.`total`, `Project2`.`state3`, `Project2`.`office2`, `Project2`.`type2`, `Project2`.`notes2`, `Project2`.`id4`, `Project2`.`refill`, `Project2`.`chip`, `Project2`.`opc_drum`, 
`Project2`.`c_blade`, `Project2`.`card_id`, `Project2`.`id5`, `Project2`.`name2`, `Project2`.`maker1`, `Project2`.`full_weight`, `Project2`.`toner_weight`, `Project2`.`resource`, 
`Project2`.`created3`, `Project2`.`user1`, `Project2`.`notes3`, `Project2`.`photo1`, `Project2`.`color1`, `Project2`.`archive`, `Project2`.`C1`, `Project2`.`id6`, `Project2`.`field_id`, 
`Project2`.`repair_id`, `Project2`.`value`, `Project2`.`item_id`, `Project2`.`id7`, `Project2`.`name3`, `Project2`.`type3`, `Project2`.`def_values`, `Project2`.`required`, 
`Project2`.`printable`, `Project2`.`archive1`, `Project2`.`_f`, `Project2`.`updated_at`, `Project2`.`devices`, `Project2`.`categories`
FROM (SELECT
`Limit1`.`id`, `Limit1`.`Hidden`, `Limit1`.`Title`, `Limit1`.`client`, `Limit1`.`type`, `Limit1`.`maker`, `Limit1`.`model`, `Limit1`.`serial_number`, `Limit1`.`company`, `Limit1`.`office`,
 `Limit1`.`start_office`, `Limit1`.`manager`, `Limit1`.`current_manager`, `Limit1`.`master`, `Limit1`.`diagnostic_result`, `Limit1`.`in_date`, `Limit1`.`out_date`, `Limit1`.`state`, 
`Limit1`.`new_state`, `Limit1`.`user_lock`, `Limit1`.`lock_datetime`, `Limit1`.`express_repair`, `Limit1`.`quick_repair`, `Limit1`.`is_warranty`, `Limit1`.`is_repeat`, 
`Limit1`.`payment_system`, `Limit1`.`is_card_payment`, `Limit1`.`can_format`, `Limit1`.`print_check`, `Limit1`.`box`, `Limit1`.`warranty_label`, `Limit1`.`ext_notes`, 
`Limit1`.`is_prepaid`, `Limit1`.`prepaid_type`, `Limit1`.`prepaid_summ`, `Limit1`.`prepaid_order`, `Limit1`.`is_pre_agreed`, `Limit1`.`is_debt`, `Limit1`.`pre_agreed_amount`, 
`Limit1`.`repair_cost`, `Limit1`.`real_repair_cost`, `Limit1`.`parts_cost`, `Limit1`.`fault`, `Limit1`.`complect`, `Limit1`.`look`, `Limit1`.`thirs_party_sc`, `Limit1`.`last_save`, 
`Limit1`.`last_status_changed`, `Limit1`.`warranty_days`, `Limit1`.`barcode`, `Limit1`.`reject_reason`, `Limit1`.`informed_status`, `Limit1`.`image_ids`, `Limit1`.`color`, 
`Limit1`.`order_moving`, `Limit1`.`early`, `Limit1`.`ext_early`, `Limit1`.`issued_msg`, `Limit1`.`sms_inform`, `Limit1`.`invoice`, `Limit1`.`cartridge`, `Limit1`.`vendor_id`, 
`Limit1`.`termsControl`, `Limit1`.`id1`, `Limit1`.`sip_user_id`, `Limit1`.`username`, `Limit1`.`name`, `Limit1`.`surname`, `Limit1`.`patronymic`, `Limit1`.`phone`, `Limit1`.`phone2`, 
`Limit1`.`phone_mask`, `Limit1`.`phone2_mask`, `Limit1`.`address`, `Limit1`.`passport_num`, `Limit1`.`passport_date`, `Limit1`.`passport_organ`, `Limit1`.`state1`, `Limit1`.`created`, 
`Limit1`.`office1`, `Limit1`.`birthday`, `Limit1`.`def_office`, `Limit1`.`def_store`, `Limit1`.`def_item_state`, `Limit1`.`def_employee`, `Limit1`.`def_status`, `Limit1`.`def_ws_filter`, 
`Limit1`.`last_login`, `Limit1`.`last_activity`, `Limit1`.`email`, `Limit1`.`sex`, `Limit1`.`photo`, `Limit1`.`salary_rate`, `Limit1`.`pay_day`, `Limit1`.`pay_day_off`, 
`Limit1`.`pay_repair`, `Limit1`.`pay_repair_quick`, `Limit1`.`pay_sale`, `Limit1`.`pay_repair_q_sale`, `Limit1`.`pay_cartridge_refill`, `Limit1`.`pay_device_in`, `Limit1`.`pay_device_out`,
 `Limit1`.`pay_4_sale_in_repair`, `Limit1`.`row_color`, `Limit1`.`ge_highlight_color`, `Limit1`.`color_label_ws`, `Limit1`.`workspace_mode`, `Limit1`.`preview_before_print`, 
`Limit1`.`new_rep_doc_copies`, `Limit1`.`auto_refresh_workspace`, `Limit1`.`refresh_time`, `Limit1`.`xls_c1`, `Limit1`.`xls_c2`, `Limit1`.`xls_c3`, `Limit1`.`xls_c4`, `Limit1`.`xls_c5`, 
`Limit1`.`xls_c6`, `Limit1`.`xls_c7`, `Limit1`.`xls_c8`, `Limit1`.`xls_c9`, `Limit1`.`xls_c10`, `Limit1`.`xls_c11`, `Limit1`.`xls_c12`, `Limit1`.`xls_c13`, `Limit1`.`xls_c14`, 
`Limit1`.`xls_c15`, `Limit1`.`display_out`, `Limit1`.`display_complete`, `Limit1`.`is_bot`, `Limit1`.`new_on_top`, `Limit1`.`issued_color`, `Limit1`.`kkm_pass`, `Limit1`.`prefer_regular`, 
`Limit1`.`fontsize`, `Limit1`.`rowheight`, `Limit1`.`animation`, `Limit1`.`fields_cfg`, `Limit1`.`save_state_on_close`, `Limit1`.`group_store_items`, `Limit1`.`track_activity`, 
`Limit1`.`card_on_call`, `Limit1`.`inn`, `Limit1`.`inform_comment`, `Limit1`.`inform_status`, `Limit1`.`kkt`, `Limit1`.`pinpad`, `Limit1`.`advance_disable`, `Limit1`.`salary_disable`, 
`Limit1`.`notes`, `Limit1`.`id2`, `Limit1`.`creator`, `Limit1`.`name1`, `Limit1`.`surname1`, `Limit1`.`patronymic1`, `Limit1`.`agent_phone_mask`, `Limit1`.`agent2_phone_mask`, 
`Limit1`.`address1`, `Limit1`.`post_index`, `Limit1`.`passport_num1`, `Limit1`.`passport_date1`, `Limit1`.`passport_organ1`, `Limit1`.`state2`, `Limit1`.`type1`, `Limit1`.`birthday1`, 
`Limit1`.`memorial`, `Limit1`.`notes1`, `Limit1`.`is_regular`, `Limit1`.`is_dealer`, `Limit1`.`balance_enable`, `Limit1`.`prefer_cashless`, `Limit1`.`take_long`, `Limit1`.`ignore_calls`, 
`Limit1`.`is_bad`, `Limit1`.`is_realizator`, `Limit1`.`is_agent`, `Limit1`.`visit_source`, `Limit1`.`photo_id`, `Limit1`.`inn1`, `Limit1`.`KPP`, `Limit1`.`OGRN`, `Limit1`.`web_password`, 
`Limit1`.`ur_name`, `Limit1`.`email1`, `Limit1`.`icq`, `Limit1`.`skype`, `Limit1`.`viber`, `Limit1`.`telegram`, `Limit1`.`site`, `Limit1`.`whatsapp`, `Limit1`.`agent_name`, 
`Limit1`.`agent_surname`, `Limit1`.`agent_patronymic`, `Limit1`.`agent_phone`, `Limit1`.`agent_phone_clean`, `Limit1`.`agent2_name`, `Limit1`.`agent2_surname`, 
`Limit1`.`agent2_patronymic`, `Limit1`.`agent2_phone`, `Limit1`.`agent2_phone_clean`, `Limit1`.`created1`, `Limit1`.`balance`, `Limit1`.`price_col`, `Limit1`.`repairs`, 
`Limit1`.`purchases`, `Limit1`.`token`, `Limit1`.`id3`, `Limit1`.`num`, `Limit1`.`created2`, `Limit1`.`user`, `Limit1`.`seller`, `Limit1`.`customer`, `Limit1`.`paid`, `Limit1`.`tax`, 
`Limit1`.`summ`, `Limit1`.`total`, `Limit1`.`state3`, `Limit1`.`office2`, `Limit1`.`type2`, `Limit1`.`notes2`, `Limit1`.`id4`, `Limit1`.`refill`, `Limit1`.`chip`, `Limit1`.`opc_drum`, 
`Limit1`.`c_blade`, `Limit1`.`card_id`, `Limit1`.`id5`, `Limit1`.`name2`, `Limit1`.`maker1`, `Limit1`.`full_weight`, `Limit1`.`toner_weight`, `Limit1`.`resource`, `Limit1`.`created3`, 
`Limit1`.`user1`, `Limit1`.`notes3`, `Limit1`.`photo1`, `Limit1`.`color1`, `Limit1`.`archive`, `Join6`.`id` AS `id6`, `Join6`.`field_id`, `Join6`.`repair_id`, `Join6`.`item_id`, 
`Join6`.`value`, `Join6`.`ID1` AS `id7`, `Join6`.`_f`, `Join6`.`name` AS `name3`, `Join6`.`type` AS `type3`, `Join6`.`def_values`, `Join6`.`required`, `Join6`.`printable`, 
`Join6`.`devices`, `Join6`.`categories`, `Join6`.`updated_at`, `Join6`.`archive` AS `archive1`, CASE WHEN (`Join6`.`id` IS NOT NULL) THEN (1)  ELSE (NULL) END AS `C1`
FROM (SELECT
`Extent1`.`id`, `Extent1`.`Hidden`, `Extent1`.`Title`, `Extent1`.`client`, `Extent1`.`type`, `Extent1`.`maker`, `Extent1`.`model`, `Extent1`.`serial_number`, `Extent1`.`company`, 
`Extent1`.`office`, `Extent1`.`start_office`, `Extent1`.`manager`, `Extent1`.`current_manager`, `Extent1`.`master`, `Extent1`.`diagnostic_result`, `Extent1`.`in_date`, 
`Extent1`.`out_date`, `Extent1`.`state`, `Extent1`.`new_state`, `Extent1`.`user_lock`, `Extent1`.`lock_datetime`, `Extent1`.`express_repair`, `Extent1`.`quick_repair`, 
`Extent1`.`is_warranty`, `Extent1`.`is_repeat`, `Extent1`.`payment_system`, `Extent1`.`is_card_payment`, `Extent1`.`can_format`, `Extent1`.`print_check`, `Extent1`.`box`, 
`Extent1`.`warranty_label`, `Extent1`.`ext_notes`, `Extent1`.`is_prepaid`, `Extent1`.`prepaid_type`, `Extent1`.`prepaid_summ`, `Extent1`.`prepaid_order`, `Extent1`.`is_pre_agreed`, 
`Extent1`.`is_debt`, `Extent1`.`pre_agreed_amount`, `Extent1`.`repair_cost`, `Extent1`.`real_repair_cost`, `Extent1`.`parts_cost`, `Extent1`.`fault`, `Extent1`.`complect`, 
`Extent1`.`look`, `Extent1`.`thirs_party_sc`, `Extent1`.`last_save`, `Extent1`.`last_status_changed`, `Extent1`.`warranty_days`, `Extent1`.`barcode`, `Extent1`.`reject_reason`, 
`Extent1`.`informed_status`, `Extent1`.`image_ids`, `Extent1`.`color`, `Extent1`.`order_moving`, `Extent1`.`early`, `Extent1`.`ext_early`, `Extent1`.`issued_msg`, `Extent1`.`sms_inform`, 
`Extent1`.`invoice`, `Extent1`.`cartridge`, `Extent1`.`vendor_id`, `Extent1`.`termsControl`, `Extent2`.`id` AS `id1`, `Extent2`.`sip_user_id`, `Extent2`.`username`, `Extent2`.`name`, 
`Extent2`.`surname`, `Extent2`.`patronymic`, `Extent2`.`phone`, `Extent2`.`phone2`, `Extent2`.`phone_mask`, `Extent2`.`phone2_mask`, `Extent2`.`address`, `Extent2`.`passport_num`, 
`Extent2`.`passport_date`, `Extent2`.`passport_organ`, `Extent2`.`state` AS `state1`, `Extent2`.`created`, `Extent2`.`office` AS `office1`, `Extent2`.`birthday`, `Extent2`.`def_office`, 
`Extent2`.`def_store`, `Extent2`.`def_item_state`, `Extent2`.`def_employee`, `Extent2`.`def_status`, `Extent2`.`def_ws_filter`, `Extent2`.`last_login`, `Extent2`.`last_activity`, 
`Extent2`.`email`, `Extent2`.`sex`, `Extent2`.`photo`, `Extent2`.`salary_rate`, `Extent2`.`pay_day`, `Extent2`.`pay_day_off`, `Extent2`.`pay_repair`, `Extent2`.`pay_repair_quick`, 
`Extent2`.`pay_sale`, `Extent2`.`pay_repair_q_sale`, `Extent2`.`pay_cartridge_refill`, `Extent2`.`pay_device_in`, `Extent2`.`pay_device_out`, `Extent2`.`pay_4_sale_in_repair`, 
`Extent2`.`row_color`, `Extent2`.`ge_highlight_color`, `Extent2`.`color_label_ws`, `Extent2`.`workspace_mode`, `Extent2`.`preview_before_print`, `Extent2`.`new_rep_doc_copies`, 
`Extent2`.`auto_refresh_workspace`, `Extent2`.`refresh_time`, `Extent2`.`xls_c1`, `Extent2`.`xls_c2`, `Extent2`.`xls_c3`, `Extent2`.`xls_c4`, `Extent2`.`xls_c5`, `Extent2`.`xls_c6`, 
`Extent2`.`xls_c7`, `Extent2`.`xls_c8`, `Extent2`.`xls_c9`, `Extent2`.`xls_c10`, `Extent2`.`xls_c11`, `Extent2`.`xls_c12`, `Extent2`.`xls_c13`, `Extent2`.`xls_c14`, `Extent2`.`xls_c15`, 
`Extent2`.`display_out`, `Extent2`.`display_complete`, `Extent2`.`is_bot`, `Extent2`.`new_on_top`, `Extent2`.`issued_color`, `Extent2`.`kkm_pass`, `Extent2`.`prefer_regular`, 
`Extent2`.`fontsize`, `Extent2`.`rowheight`, `Extent2`.`animation`, `Extent2`.`fields_cfg`, `Extent2`.`save_state_on_close`, `Extent2`.`group_store_items`, `Extent2`.`track_activity`, 
`Extent2`.`card_on_call`, `Extent2`.`inn`, `Extent2`.`inform_comment`, `Extent2`.`inform_status`, `Extent2`.`kkt`, `Extent2`.`pinpad`, `Extent2`.`advance_disable`, 
`Extent2`.`salary_disable`, `Extent2`.`notes`, `Extent3`.`id` AS `id2`, `Extent3`.`creator`, `Extent3`.`name` AS `name1`, `Extent3`.`surname` AS `surname1`, `Extent3`.`patronymic` AS 
`patronymic1`, `Extent3`.`agent_phone_mask`, `Extent3`.`agent2_phone_mask`, `Extent3`.`address` AS `address1`, `Extent3`.`post_index`, `Extent3`.`passport_num` AS `passport_num1`, 
`Extent3`.`passport_date` AS `passport_date1`, `Extent3`.`passport_organ` AS `passport_organ1`, `Extent3`.`state` AS `state2`, `Extent3`.`type` AS `type1`, `Extent3`.`birthday` AS 
`birthday1`, `Extent3`.`memorial`, `Extent3`.`notes` AS `notes1`, `Extent3`.`is_regular`, `Extent3`.`is_dealer`, `Extent3`.`balance_enable`, `Extent3`.`prefer_cashless`, 
`Extent3`.`take_long`, `Extent3`.`ignore_calls`, `Extent3`.`is_bad`, `Extent3`.`is_realizator`, `Extent3`.`is_agent`, `Extent3`.`visit_source`, `Extent3`.`photo_id`, `Extent3`.`INN` AS 
`inn1`, `Extent3`.`KPP`, `Extent3`.`OGRN`, `Extent3`.`web_password`, `Extent3`.`ur_name`, `Extent3`.`email` AS `email1`, `Extent3`.`icq`, `Extent3`.`skype`, `Extent3`.`viber`, 
`Extent3`.`telegram`, `Extent3`.`site`, `Extent3`.`whatsapp`, `Extent3`.`agent_name`, `Extent3`.`agent_surname`, `Extent3`.`agent_patronymic`, `Extent3`.`agent_phone`, 
`Extent3`.`agent_phone_clean`, `Extent3`.`agent2_name`, `Extent3`.`agent2_surname`, `Extent3`.`agent2_patronymic`, `Extent3`.`agent2_phone`, `Extent3`.`agent2_phone_clean`, 
`Extent3`.`created` AS `created1`, `Extent3`.`balance`, `Extent3`.`price_col`, `Extent3`.`repairs`, `Extent3`.`purchases`, `Extent3`.`token`, `Extent4`.`id` AS `id3`, `Extent4`.`num`, 
`Extent4`.`created` AS `created2`, `Extent4`.`user`, `Extent4`.`seller`, `Extent4`.`customer`, `Extent4`.`paid`, `Extent4`.`tax`, `Extent4`.`summ`, `Extent4`.`total`, `Extent4`.`state` AS 
`state3`, `Extent4`.`office` AS `office2`, `Extent4`.`type` AS `type2`, `Extent4`.`notes` AS `notes2`, `Extent5`.`id` AS `id4`, `Extent5`.`refill`, `Extent5`.`chip`, `Extent5`.`opc_drum`, 
`Extent5`.`c_blade`, `Extent5`.`card_id`, `Extent6`.`id` AS `id5`, `Extent6`.`name` AS `name2`, `Extent6`.`maker` AS `maker1`, `Extent6`.`full_weight`, `Extent6`.`toner_weight`, 
`Extent6`.`resource`, `Extent6`.`created` AS `created3`, `Extent6`.`user` AS `user1`, `Extent6`.`notes` AS `notes3`, `Extent6`.`photo` AS `photo1`, `Extent6`.`color` AS `color1`, 
`Extent6`.`archive`
FROM `workshop` AS `Extent1` LEFT OUTER JOIN `users` AS `Extent2` ON `Extent1`.`user_lock` = `Extent2`.`id` INNER JOIN `clients` AS `Extent3` ON `Extent1`.`client` = `Extent3`.`id` LEFT OUTER JOIN `invoice` AS `Extent4` ON `Extent1`.`invoice` = `Extent4`.`id` LEFT OUTER JOIN `c_workshop` AS `Extent5` ON `Extent1`.`cartridge` = `Extent5`.`id` LEFT OUTER JOIN `cartridge_cards` AS `Extent6` ON `Extent5`.`card_id` = `Extent6`.`id`
 WHERE `Extent1`.`id` = 24875 LIMIT 2) AS `Limit1` LEFT OUTER JOIN (SELECT
`Extent7`.`id`, `Extent7`.`field_id`, `Extent7`.`repair_id`, `Extent7`.`item_id`, `Extent7`.`value`, `Extent8`.`id` AS `ID1`, `Extent8`.`_f`, `Extent8`.`name`, `Extent8`.`type`, 
`Extent8`.`def_values`, `Extent8`.`required`, `Extent8`.`printable`, `Extent8`.`devices`, `Extent8`.`categories`, `Extent8`.`updated_at`, `Extent8`.`archive`
FROM `field_values` AS `Extent7` INNER JOIN `fields` AS `Extent8` ON `Extent7`.`field_id` = `Extent8`.`id`) AS `Join6` ON `Limit1`.`id` = `Join6`.`repair_id`) AS `Project2`
 ORDER BY 
`Project2`.`id` ASC, `Project2`.`id1` ASC, `Project2`.`id2` ASC, `Project2`.`id3` ASC, `Project2`.`id4` ASC, `Project2`.`id5` ASC, `Project2`.`C1` ASC;


BEGIN;

UPDATE `workshop` SET `user_lock`=32, `lock_datetime`='2022-03-24 23:48:39.273475' WHERE `id` = 24875;

COMMIT;

#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# Выдача без ремонта (без запчастей)
argument;

SELECT
`Project2`.`id`, `Project2`.`client`, `Project2`.`type`, `Project2`.`maker`, `Project2`.`model`, `Project2`.`serial_number`, `Project2`.`company`, `Project2`.`office`, 
`Project2`.`start_office`, `Project2`.`manager`, `Project2`.`current_manager`, `Project2`.`master`, `Project2`.`diagnostic_result`, `Project2`.`in_date`, `Project2`.`out_date`, 
`Project2`.`state`, `Project2`.`user_lock`, `Project2`.`lock_datetime`, `Project2`.`express_repair`, `Project2`.`is_warranty`, `Project2`.`is_repeat`, `Project2`.`can_format`, 
`Project2`.`print_check`, `Project2`.`box`, `Project2`.`warranty_label`, `Project2`.`ext_notes`, `Project2`.`is_prepaid`, `Project2`.`prepaid_type`, `Project2`.`prepaid_summ`, 
`Project2`.`prepaid_order`, `Project2`.`is_pre_agreed`, `Project2`.`pre_agreed_amount`, `Project2`.`repair_cost`, `Project2`.`fault`, `Project2`.`complect`, `Project2`.`look`, 
`Project2`.`thirs_party_sc`, `Project2`.`last_save`, `Project2`.`last_status_changed`, `Project2`.`warranty_days`, `Project2`.`barcode`, `Project2`.`new_state`, 
`Project2`.`is_card_payment`, `Project2`.`informed_status`, `Project2`.`parts_cost`, `Project2`.`is_debt`, `Project2`.`image_ids`, `Project2`.`color`, `Project2`.`order_moving`, 
`Project2`.`payment_system`, `Project2`.`early`, `Project2`.`real_repair_cost`, `Project2`.`issued_msg`, `Project2`.`reject_reason`, `Project2`.`ext_early`, `Project2`.`sms_inform`, 
`Project2`.`invoice`, `Project2`.`cartridge`, `Project2`.`termsControl`, `Project2`.`vendor_id`, `Project2`.`quick_repair`, `Project2`.`Hidden`, `Project2`.`Title`, `Project2`.`id1`, 
`Project2`.`sip_user_id`, `Project2`.`username`, `Project2`.`name`, `Project2`.`surname`, `Project2`.`patronymic`, `Project2`.`phone`, `Project2`.`phone2`, `Project2`.`phone_mask`, 
`Project2`.`phone2_mask`, `Project2`.`address`, `Project2`.`passport_num`, `Project2`.`passport_date`, `Project2`.`passport_organ`, `Project2`.`state1`, `Project2`.`created`, 
`Project2`.`office1`, `Project2`.`birthday`, `Project2`.`last_login`, `Project2`.`last_activity`, `Project2`.`email`, `Project2`.`sex`, `Project2`.`photo`, `Project2`.`salary_rate`, 
`Project2`.`pay_day`, `Project2`.`pay_day_off`, `Project2`.`pay_repair`, `Project2`.`pay_sale`, `Project2`.`pay_repair_q_sale`, `Project2`.`pay_cartridge_refill`, 
`Project2`.`color_label_ws`, `Project2`.`workspace_mode`, `Project2`.`preview_before_print`, `Project2`.`new_rep_doc_copies`, `Project2`.`auto_refresh_workspace`, 
`Project2`.`refresh_time`, `Project2`.`xls_c1`, `Project2`.`xls_c2`, `Project2`.`xls_c3`, `Project2`.`xls_c4`, `Project2`.`xls_c5`, `Project2`.`xls_c6`, `Project2`.`xls_c7`, 
`Project2`.`xls_c8`, `Project2`.`xls_c9`, `Project2`.`xls_c10`, `Project2`.`xls_c11`, `Project2`.`xls_c12`, `Project2`.`xls_c13`, `Project2`.`xls_c14`, `Project2`.`xls_c15`, 
`Project2`.`display_out`, `Project2`.`display_complete`, `Project2`.`is_bot`, `Project2`.`new_on_top`, `Project2`.`issued_color`, `Project2`.`fields_cfg`, `Project2`.`kkm_pass`, 
`Project2`.`prefer_regular`, `Project2`.`fontsize`, `Project2`.`rowheight`, `Project2`.`animation`, `Project2`.`pay_device_in`, `Project2`.`pay_device_out`, 
`Project2`.`pay_4_sale_in_repair`, `Project2`.`row_color`, `Project2`.`save_state_on_close`, `Project2`.`group_store_items`, `Project2`.`track_activity`, `Project2`.`inn`, 
`Project2`.`inform_comment`, `Project2`.`inform_status`, `Project2`.`kkt`, `Project2`.`pinpad`, `Project2`.`def_office`, `Project2`.`def_store`, `Project2`.`def_item_state`, 
`Project2`.`def_employee`, `Project2`.`def_status`, `Project2`.`def_ws_filter`, `Project2`.`card_on_call`, `Project2`.`ge_highlight_color`, `Project2`.`pay_repair_quick`, 
`Project2`.`advance_disable`, `Project2`.`salary_disable`, `Project2`.`notes`, `Project2`.`id2`, `Project2`.`creator`, `Project2`.`name1`, `Project2`.`surname1`, `Project2`.`patronymic1`, 
`Project2`.`agent_phone_mask`, `Project2`.`agent2_phone_mask`, `Project2`.`address1`, `Project2`.`post_index`, `Project2`.`passport_num1`, `Project2`.`passport_date1`, 
`Project2`.`passport_organ1`, `Project2`.`state2`, `Project2`.`type1`, `Project2`.`birthday1`, `Project2`.`memorial`, `Project2`.`notes1`, `Project2`.`is_regular`, `Project2`.`is_dealer`, 
`Project2`.`balance_enable`, `Project2`.`prefer_cashless`, `Project2`.`take_long`, `Project2`.`ignore_calls`, `Project2`.`is_bad`, `Project2`.`is_realizator`, `Project2`.`is_agent`, 
`Project2`.`inn1`, `Project2`.`KPP`, `Project2`.`OGRN`, `Project2`.`web_password`, `Project2`.`ur_name`, `Project2`.`email1`, `Project2`.`icq`, `Project2`.`skype`, `Project2`.`viber`, 
`Project2`.`telegram`, `Project2`.`site`, `Project2`.`whatsapp`, `Project2`.`agent_name`, `Project2`.`agent_surname`, `Project2`.`agent_patronymic`, `Project2`.`agent_phone`, 
`Project2`.`agent2_name`, `Project2`.`agent2_surname`, `Project2`.`agent2_patronymic`, `Project2`.`agent2_phone`, `Project2`.`created1`, `Project2`.`balance`, `Project2`.`price_col`, 
`Project2`.`agent_phone_clean`, `Project2`.`agent2_phone_clean`, `Project2`.`repairs`, `Project2`.`token`, `Project2`.`purchases`, `Project2`.`visit_source`, `Project2`.`photo_id`, 
`Project2`.`id3`, `Project2`.`num`, `Project2`.`created2`, `Project2`.`user`, `Project2`.`seller`, `Project2`.`customer`, `Project2`.`paid`, `Project2`.`tax`, `Project2`.`summ`, 
`Project2`.`total`, `Project2`.`state3`, `Project2`.`office2`, `Project2`.`type2`, `Project2`.`notes2`, `Project2`.`id4`, `Project2`.`refill`, `Project2`.`chip`, `Project2`.`opc_drum`, 
`Project2`.`c_blade`, `Project2`.`card_id`, `Project2`.`id5`, `Project2`.`name2`, `Project2`.`maker1`, `Project2`.`full_weight`, `Project2`.`toner_weight`, `Project2`.`resource`, 
`Project2`.`created3`, `Project2`.`user1`, `Project2`.`notes3`, `Project2`.`photo1`, `Project2`.`color1`, `Project2`.`archive`, `Project2`.`C1`, `Project2`.`id6`, `Project2`.`field_id`, 
`Project2`.`repair_id`, `Project2`.`value`, `Project2`.`item_id`, `Project2`.`id7`, `Project2`.`name3`, `Project2`.`type3`, `Project2`.`def_values`, `Project2`.`required`, 
`Project2`.`printable`, `Project2`.`archive1`, `Project2`.`_f`, `Project2`.`updated_at`, `Project2`.`devices`, `Project2`.`categories`
FROM (SELECT
`Limit1`.`id`, `Limit1`.`Hidden`, `Limit1`.`Title`, `Limit1`.`client`, `Limit1`.`type`, `Limit1`.`maker`, `Limit1`.`model`, `Limit1`.`serial_number`, `Limit1`.`company`, `Limit1`.`office`,
 `Limit1`.`start_office`, `Limit1`.`manager`, `Limit1`.`current_manager`, `Limit1`.`master`, `Limit1`.`diagnostic_result`, `Limit1`.`in_date`, `Limit1`.`out_date`, `Limit1`.`state`, 
`Limit1`.`new_state`, `Limit1`.`user_lock`, `Limit1`.`lock_datetime`, `Limit1`.`express_repair`, `Limit1`.`quick_repair`, `Limit1`.`is_warranty`, `Limit1`.`is_repeat`, 
`Limit1`.`payment_system`, `Limit1`.`is_card_payment`, `Limit1`.`can_format`, `Limit1`.`print_check`, `Limit1`.`box`, `Limit1`.`warranty_label`, `Limit1`.`ext_notes`, 
`Limit1`.`is_prepaid`, `Limit1`.`prepaid_type`, `Limit1`.`prepaid_summ`, `Limit1`.`prepaid_order`, `Limit1`.`is_pre_agreed`, `Limit1`.`is_debt`, `Limit1`.`pre_agreed_amount`, 
`Limit1`.`repair_cost`, `Limit1`.`real_repair_cost`, `Limit1`.`parts_cost`, `Limit1`.`fault`, `Limit1`.`complect`, `Limit1`.`look`, `Limit1`.`thirs_party_sc`, `Limit1`.`last_save`, 
`Limit1`.`last_status_changed`, `Limit1`.`warranty_days`, `Limit1`.`barcode`, `Limit1`.`reject_reason`, `Limit1`.`informed_status`, `Limit1`.`image_ids`, `Limit1`.`color`, 
`Limit1`.`order_moving`, `Limit1`.`early`, `Limit1`.`ext_early`, `Limit1`.`issued_msg`, `Limit1`.`sms_inform`, `Limit1`.`invoice`, `Limit1`.`cartridge`, `Limit1`.`vendor_id`, 
`Limit1`.`termsControl`, `Limit1`.`id1`, `Limit1`.`sip_user_id`, `Limit1`.`username`, `Limit1`.`name`, `Limit1`.`surname`, `Limit1`.`patronymic`, `Limit1`.`phone`, `Limit1`.`phone2`, 
`Limit1`.`phone_mask`, `Limit1`.`phone2_mask`, `Limit1`.`address`, `Limit1`.`passport_num`, `Limit1`.`passport_date`, `Limit1`.`passport_organ`, `Limit1`.`state1`, `Limit1`.`created`, 
`Limit1`.`office1`, `Limit1`.`birthday`, `Limit1`.`def_office`, `Limit1`.`def_store`, `Limit1`.`def_item_state`, `Limit1`.`def_employee`, `Limit1`.`def_status`, `Limit1`.`def_ws_filter`, 
`Limit1`.`last_login`, `Limit1`.`last_activity`, `Limit1`.`email`, `Limit1`.`sex`, `Limit1`.`photo`, `Limit1`.`salary_rate`, `Limit1`.`pay_day`, `Limit1`.`pay_day_off`, 
`Limit1`.`pay_repair`, `Limit1`.`pay_repair_quick`, `Limit1`.`pay_sale`, `Limit1`.`pay_repair_q_sale`, `Limit1`.`pay_cartridge_refill`, `Limit1`.`pay_device_in`, `Limit1`.`pay_device_out`,
 `Limit1`.`pay_4_sale_in_repair`, `Limit1`.`row_color`, `Limit1`.`ge_highlight_color`, `Limit1`.`color_label_ws`, `Limit1`.`workspace_mode`, `Limit1`.`preview_before_print`, 
`Limit1`.`new_rep_doc_copies`, `Limit1`.`auto_refresh_workspace`, `Limit1`.`refresh_time`, `Limit1`.`xls_c1`, `Limit1`.`xls_c2`, `Limit1`.`xls_c3`, `Limit1`.`xls_c4`, `Limit1`.`xls_c5`, 
`Limit1`.`xls_c6`, `Limit1`.`xls_c7`, `Limit1`.`xls_c8`, `Limit1`.`xls_c9`, `Limit1`.`xls_c10`, `Limit1`.`xls_c11`, `Limit1`.`xls_c12`, `Limit1`.`xls_c13`, `Limit1`.`xls_c14`, 
`Limit1`.`xls_c15`, `Limit1`.`display_out`, `Limit1`.`display_complete`, `Limit1`.`is_bot`, `Limit1`.`new_on_top`, `Limit1`.`issued_color`, `Limit1`.`kkm_pass`, `Limit1`.`prefer_regular`, 
`Limit1`.`fontsize`, `Limit1`.`rowheight`, `Limit1`.`animation`, `Limit1`.`fields_cfg`, `Limit1`.`save_state_on_close`, `Limit1`.`group_store_items`, `Limit1`.`track_activity`, 
`Limit1`.`card_on_call`, `Limit1`.`inn`, `Limit1`.`inform_comment`, `Limit1`.`inform_status`, `Limit1`.`kkt`, `Limit1`.`pinpad`, `Limit1`.`advance_disable`, `Limit1`.`salary_disable`, 
`Limit1`.`notes`, `Limit1`.`id2`, `Limit1`.`creator`, `Limit1`.`name1`, `Limit1`.`surname1`, `Limit1`.`patronymic1`, `Limit1`.`agent_phone_mask`, `Limit1`.`agent2_phone_mask`, 
`Limit1`.`address1`, `Limit1`.`post_index`, `Limit1`.`passport_num1`, `Limit1`.`passport_date1`, `Limit1`.`passport_organ1`, `Limit1`.`state2`, `Limit1`.`type1`, `Limit1`.`birthday1`, 
`Limit1`.`memorial`, `Limit1`.`notes1`, `Limit1`.`is_regular`, `Limit1`.`is_dealer`, `Limit1`.`balance_enable`, `Limit1`.`prefer_cashless`, `Limit1`.`take_long`, `Limit1`.`ignore_calls`, 
`Limit1`.`is_bad`, `Limit1`.`is_realizator`, `Limit1`.`is_agent`, `Limit1`.`visit_source`, `Limit1`.`photo_id`, `Limit1`.`inn1`, `Limit1`.`KPP`, `Limit1`.`OGRN`, `Limit1`.`web_password`, 
`Limit1`.`ur_name`, `Limit1`.`email1`, `Limit1`.`icq`, `Limit1`.`skype`, `Limit1`.`viber`, `Limit1`.`telegram`, `Limit1`.`site`, `Limit1`.`whatsapp`, `Limit1`.`agent_name`, 
`Limit1`.`agent_surname`, `Limit1`.`agent_patronymic`, `Limit1`.`agent_phone`, `Limit1`.`agent_phone_clean`, `Limit1`.`agent2_name`, `Limit1`.`agent2_surname`, 
`Limit1`.`agent2_patronymic`, `Limit1`.`agent2_phone`, `Limit1`.`agent2_phone_clean`, `Limit1`.`created1`, `Limit1`.`balance`, `Limit1`.`price_col`, `Limit1`.`repairs`, 
`Limit1`.`purchases`, `Limit1`.`token`, `Limit1`.`id3`, `Limit1`.`num`, `Limit1`.`created2`, `Limit1`.`user`, `Limit1`.`seller`, `Limit1`.`customer`, `Limit1`.`paid`, `Limit1`.`tax`, 
`Limit1`.`summ`, `Limit1`.`total`, `Limit1`.`state3`, `Limit1`.`office2`, `Limit1`.`type2`, `Limit1`.`notes2`, `Limit1`.`id4`, `Limit1`.`refill`, `Limit1`.`chip`, `Limit1`.`opc_drum`, 
`Limit1`.`c_blade`, `Limit1`.`card_id`, `Limit1`.`id5`, `Limit1`.`name2`, `Limit1`.`maker1`, `Limit1`.`full_weight`, `Limit1`.`toner_weight`, `Limit1`.`resource`, `Limit1`.`created3`, 
`Limit1`.`user1`, `Limit1`.`notes3`, `Limit1`.`photo1`, `Limit1`.`color1`, `Limit1`.`archive`, `Join6`.`id` AS `id6`, `Join6`.`field_id`, `Join6`.`repair_id`, `Join6`.`item_id`, 
`Join6`.`value`, `Join6`.`ID1` AS `id7`, `Join6`.`_f`, `Join6`.`name` AS `name3`, `Join6`.`type` AS `type3`, `Join6`.`def_values`, `Join6`.`required`, `Join6`.`printable`, 
`Join6`.`devices`, `Join6`.`categories`, `Join6`.`updated_at`, `Join6`.`archive` AS `archive1`, CASE WHEN (`Join6`.`id` IS NOT NULL) THEN (1)  ELSE (NULL) END AS `C1`
FROM (SELECT
`Extent1`.`id`, `Extent1`.`Hidden`, `Extent1`.`Title`, `Extent1`.`client`, `Extent1`.`type`, `Extent1`.`maker`, `Extent1`.`model`, `Extent1`.`serial_number`, `Extent1`.`company`, 
`Extent1`.`office`, `Extent1`.`start_office`, `Extent1`.`manager`, `Extent1`.`current_manager`, `Extent1`.`master`, `Extent1`.`diagnostic_result`, `Extent1`.`in_date`, 
`Extent1`.`out_date`, `Extent1`.`state`, `Extent1`.`new_state`, `Extent1`.`user_lock`, `Extent1`.`lock_datetime`, `Extent1`.`express_repair`, `Extent1`.`quick_repair`, 
`Extent1`.`is_warranty`, `Extent1`.`is_repeat`, `Extent1`.`payment_system`, `Extent1`.`is_card_payment`, `Extent1`.`can_format`, `Extent1`.`print_check`, `Extent1`.`box`, 
`Extent1`.`warranty_label`, `Extent1`.`ext_notes`, `Extent1`.`is_prepaid`, `Extent1`.`prepaid_type`, `Extent1`.`prepaid_summ`, `Extent1`.`prepaid_order`, `Extent1`.`is_pre_agreed`, 
`Extent1`.`is_debt`, `Extent1`.`pre_agreed_amount`, `Extent1`.`repair_cost`, `Extent1`.`real_repair_cost`, `Extent1`.`parts_cost`, `Extent1`.`fault`, `Extent1`.`complect`, 
`Extent1`.`look`, `Extent1`.`thirs_party_sc`, `Extent1`.`last_save`, `Extent1`.`last_status_changed`, `Extent1`.`warranty_days`, `Extent1`.`barcode`, `Extent1`.`reject_reason`, 
`Extent1`.`informed_status`, `Extent1`.`image_ids`, `Extent1`.`color`, `Extent1`.`order_moving`, `Extent1`.`early`, `Extent1`.`ext_early`, `Extent1`.`issued_msg`, `Extent1`.`sms_inform`, 
`Extent1`.`invoice`, `Extent1`.`cartridge`, `Extent1`.`vendor_id`, `Extent1`.`termsControl`, `Extent2`.`id` AS `id1`, `Extent2`.`sip_user_id`, `Extent2`.`username`, `Extent2`.`name`, 
`Extent2`.`surname`, `Extent2`.`patronymic`, `Extent2`.`phone`, `Extent2`.`phone2`, `Extent2`.`phone_mask`, `Extent2`.`phone2_mask`, `Extent2`.`address`, `Extent2`.`passport_num`, 
`Extent2`.`passport_date`, `Extent2`.`passport_organ`, `Extent2`.`state` AS `state1`, `Extent2`.`created`, `Extent2`.`office` AS `office1`, `Extent2`.`birthday`, `Extent2`.`def_office`, 
`Extent2`.`def_store`, `Extent2`.`def_item_state`, `Extent2`.`def_employee`, `Extent2`.`def_status`, `Extent2`.`def_ws_filter`, `Extent2`.`last_login`, `Extent2`.`last_activity`, 
`Extent2`.`email`, `Extent2`.`sex`, `Extent2`.`photo`, `Extent2`.`salary_rate`, `Extent2`.`pay_day`, `Extent2`.`pay_day_off`, `Extent2`.`pay_repair`, `Extent2`.`pay_repair_quick`, 
`Extent2`.`pay_sale`, `Extent2`.`pay_repair_q_sale`, `Extent2`.`pay_cartridge_refill`, `Extent2`.`pay_device_in`, `Extent2`.`pay_device_out`, `Extent2`.`pay_4_sale_in_repair`, 
`Extent2`.`row_color`, `Extent2`.`ge_highlight_color`, `Extent2`.`color_label_ws`, `Extent2`.`workspace_mode`, `Extent2`.`preview_before_print`, `Extent2`.`new_rep_doc_copies`, 
`Extent2`.`auto_refresh_workspace`, `Extent2`.`refresh_time`, `Extent2`.`xls_c1`, `Extent2`.`xls_c2`, `Extent2`.`xls_c3`, `Extent2`.`xls_c4`, `Extent2`.`xls_c5`, `Extent2`.`xls_c6`, 
`Extent2`.`xls_c7`, `Extent2`.`xls_c8`, `Extent2`.`xls_c9`, `Extent2`.`xls_c10`, `Extent2`.`xls_c11`, `Extent2`.`xls_c12`, `Extent2`.`xls_c13`, `Extent2`.`xls_c14`, `Extent2`.`xls_c15`, 
`Extent2`.`display_out`, `Extent2`.`display_complete`, `Extent2`.`is_bot`, `Extent2`.`new_on_top`, `Extent2`.`issued_color`, `Extent2`.`kkm_pass`, `Extent2`.`prefer_regular`, 
`Extent2`.`fontsize`, `Extent2`.`rowheight`, `Extent2`.`animation`, `Extent2`.`fields_cfg`, `Extent2`.`save_state_on_close`, `Extent2`.`group_store_items`, `Extent2`.`track_activity`, 
`Extent2`.`card_on_call`, `Extent2`.`inn`, `Extent2`.`inform_comment`, `Extent2`.`inform_status`, `Extent2`.`kkt`, `Extent2`.`pinpad`, `Extent2`.`advance_disable`, 
`Extent2`.`salary_disable`, `Extent2`.`notes`, `Extent3`.`id` AS `id2`, `Extent3`.`creator`, `Extent3`.`name` AS `name1`, `Extent3`.`surname` AS `surname1`, `Extent3`.`patronymic` AS 
`patronymic1`, `Extent3`.`agent_phone_mask`, `Extent3`.`agent2_phone_mask`, `Extent3`.`address` AS `address1`, `Extent3`.`post_index`, `Extent3`.`passport_num` AS `passport_num1`, 
`Extent3`.`passport_date` AS `passport_date1`, `Extent3`.`passport_organ` AS `passport_organ1`, `Extent3`.`state` AS `state2`, `Extent3`.`type` AS `type1`, `Extent3`.`birthday` AS 
`birthday1`, `Extent3`.`memorial`, `Extent3`.`notes` AS `notes1`, `Extent3`.`is_regular`, `Extent3`.`is_dealer`, `Extent3`.`balance_enable`, `Extent3`.`prefer_cashless`, 
`Extent3`.`take_long`, `Extent3`.`ignore_calls`, `Extent3`.`is_bad`, `Extent3`.`is_realizator`, `Extent3`.`is_agent`, `Extent3`.`visit_source`, `Extent3`.`photo_id`, `Extent3`.`INN` AS 
`inn1`, `Extent3`.`KPP`, `Extent3`.`OGRN`, `Extent3`.`web_password`, `Extent3`.`ur_name`, `Extent3`.`email` AS `email1`, `Extent3`.`icq`, `Extent3`.`skype`, `Extent3`.`viber`, 
`Extent3`.`telegram`, `Extent3`.`site`, `Extent3`.`whatsapp`, `Extent3`.`agent_name`, `Extent3`.`agent_surname`, `Extent3`.`agent_patronymic`, `Extent3`.`agent_phone`, 
`Extent3`.`agent_phone_clean`, `Extent3`.`agent2_name`, `Extent3`.`agent2_surname`, `Extent3`.`agent2_patronymic`, `Extent3`.`agent2_phone`, `Extent3`.`agent2_phone_clean`, 
`Extent3`.`created` AS `created1`, `Extent3`.`balance`, `Extent3`.`price_col`, `Extent3`.`repairs`, `Extent3`.`purchases`, `Extent3`.`token`, `Extent4`.`id` AS `id3`, `Extent4`.`num`, 
`Extent4`.`created` AS `created2`, `Extent4`.`user`, `Extent4`.`seller`, `Extent4`.`customer`, `Extent4`.`paid`, `Extent4`.`tax`, `Extent4`.`summ`, `Extent4`.`total`, `Extent4`.`state` AS 
`state3`, `Extent4`.`office` AS `office2`, `Extent4`.`type` AS `type2`, `Extent4`.`notes` AS `notes2`, `Extent5`.`id` AS `id4`, `Extent5`.`refill`, `Extent5`.`chip`, `Extent5`.`opc_drum`, 
`Extent5`.`c_blade`, `Extent5`.`card_id`, `Extent6`.`id` AS `id5`, `Extent6`.`name` AS `name2`, `Extent6`.`maker` AS `maker1`, `Extent6`.`full_weight`, `Extent6`.`toner_weight`, 
`Extent6`.`resource`, `Extent6`.`created` AS `created3`, `Extent6`.`user` AS `user1`, `Extent6`.`notes` AS `notes3`, `Extent6`.`photo` AS `photo1`, `Extent6`.`color` AS `color1`, 
`Extent6`.`archive`
FROM `workshop` AS `Extent1` LEFT OUTER JOIN `users` AS `Extent2` ON `Extent1`.`user_lock` = `Extent2`.`id` INNER JOIN `clients` AS `Extent3` ON `Extent1`.`client` = `Extent3`.`id` LEFT OUTER JOIN `invoice` AS `Extent4` ON `Extent1`.`invoice` = `Extent4`.`id` LEFT OUTER JOIN `c_workshop` AS `Extent5` ON `Extent1`.`cartridge` = `Extent5`.`id` LEFT OUTER JOIN `cartridge_cards` AS `Extent6` ON `Extent5`.`card_id` = `Extent6`.`id`
 WHERE `Extent1`.`id` = 24857 LIMIT 2) AS `Limit1` LEFT OUTER JOIN (SELECT
`Extent7`.`id`, `Extent7`.`field_id`, `Extent7`.`repair_id`, `Extent7`.`item_id`, `Extent7`.`value`, `Extent8`.`id` AS `ID1`, `Extent8`.`_f`, `Extent8`.`name`, `Extent8`.`type`, 
`Extent8`.`def_values`, `Extent8`.`required`, `Extent8`.`printable`, `Extent8`.`devices`, `Extent8`.`categories`, `Extent8`.`updated_at`, `Extent8`.`archive`
FROM `field_values` AS `Extent7` INNER JOIN `fields` AS `Extent8` ON `Extent7`.`field_id` = `Extent8`.`id`) AS `Join6` ON `Limit1`.`id` = `Join6`.`repair_id`) AS `Project2`
 ORDER BY 
`Project2`.`id` ASC, `Project2`.`id1` ASC, `Project2`.`id2` ASC, `Project2`.`id3` ASC, `Project2`.`id4` ASC, `Project2`.`id5` ASC, `Project2`.`C1` ASC;

SELECT
`GroupBy1`.`A1` AS `C1`
FROM (SELECT
SUM(CASE WHEN (`Project1`.`C1` IS NOT NULL) THEN (`Project1`.`summa`)  ELSE (0) END) AS `A1`
FROM (SELECT
1 AS `X`) AS `SingleRowTable1` LEFT OUTER JOIN (SELECT
`Extent1`.`summa`, 
1 AS `C1`
FROM `cash_orders` AS `Extent1`
 WHERE `Extent1`.`repair` = 24857) AS `Project1` ON 1 = 1) AS `GroupBy1`;

SELECT
`GroupBy1`.`A1` AS `C1`
FROM (SELECT
SUM(CASE WHEN (`Project1`.`C2` IS NOT NULL) THEN (`Project1`.`C1`)  ELSE (0) END) AS `A1`
FROM (SELECT
1 AS `X`) AS `SingleRowTable1` LEFT OUTER JOIN (SELECT
`Extent1`.`price` * (`Extent1`.`count`) AS `C1`, 
1 AS `C2`
FROM `works` AS `Extent1`
 WHERE `Extent1`.`repair` = 24857) AS `Project1` ON 1 = 1) AS `GroupBy1`;

SELECT
`GroupBy1`.`A1` AS `C1`
FROM (SELECT
SUM(CASE WHEN (`Project1`.`C2` IS NOT NULL) THEN (`Project1`.`C1`)  ELSE (0) END) AS `A1`
FROM (SELECT
1 AS `X`) AS `SingleRowTable1` LEFT OUTER JOIN (SELECT
(`Extent1`.`count`) * `Extent1`.`price` AS `C1`, 
1 AS `C2`
FROM `store_int_reserve` AS `Extent1`
 WHERE (`Extent1`.`repair_id` = 24857) AND ((2 = `Extent1`.`state`) OR (3 = `Extent1`.`state`))) AS `Project1` ON 1 = 1) AS `GroupBy1`;

SELECT
`GroupBy1`.`A1` AS `C1`
FROM (SELECT
SUM(CASE WHEN (`Project1`.`C1` IS NOT NULL) THEN (`Project1`.`summa`)  ELSE (0) END) AS `A1`
FROM (SELECT
1 AS `X`) AS `SingleRowTable1` LEFT OUTER JOIN (SELECT
`Extent1`.`summa`, 
1 AS `C1`
FROM `cash_orders` AS `Extent1`
 WHERE `Extent1`.`repair` = 24857) AS `Project1` ON 1 = 1) AS `GroupBy1`;

SELECT
`GroupBy1`.`A1` AS `C1`
FROM (SELECT
SUM(CASE WHEN (`Project1`.`C2` IS NOT NULL) THEN (`Project1`.`C1`)  ELSE (0) END) AS `A1`
FROM (SELECT
1 AS `X`) AS `SingleRowTable1` LEFT OUTER JOIN (SELECT
`Extent1`.`price` * (`Extent1`.`count`) AS `C1`, 
1 AS `C2`
FROM `works` AS `Extent1`
 WHERE `Extent1`.`repair` = 24857) AS `Project1` ON 1 = 1) AS `GroupBy1`;

SELECT
`GroupBy1`.`A1` AS `C1`
FROM (SELECT
SUM(CASE WHEN (`Project1`.`C2` IS NOT NULL) THEN (`Project1`.`C1`)  ELSE (0) END) AS `A1`
FROM (SELECT
1 AS `X`) AS `SingleRowTable1` LEFT OUTER JOIN (SELECT
(`Extent1`.`count`) * `Extent1`.`price` AS `C1`, 
1 AS `C2`
FROM `store_int_reserve` AS `Extent1`
 WHERE (`Extent1`.`repair_id` = 24857) AND ((2 = `Extent1`.`state`) OR (3 = `Extent1`.`state`))) AS `Project1` ON 1 = 1) AS `GroupBy1`;

SELECT
`Project2`.`id`, `Project2`.`creator`, `Project2`.`name`, `Project2`.`surname`, `Project2`.`patronymic`, `Project2`.`agent_phone_mask`, `Project2`.`agent2_phone_mask`, 
`Project2`.`address`, `Project2`.`post_index`, `Project2`.`passport_num`, `Project2`.`passport_date`, `Project2`.`passport_organ`, `Project2`.`state`, `Project2`.`type`, 
`Project2`.`birthday`, `Project2`.`memorial`, `Project2`.`notes`, `Project2`.`is_regular`, `Project2`.`is_dealer`, `Project2`.`balance_enable`, `Project2`.`prefer_cashless`, 
`Project2`.`take_long`, `Project2`.`ignore_calls`, `Project2`.`is_bad`, `Project2`.`is_realizator`, `Project2`.`is_agent`, `Project2`.`INN`, `Project2`.`KPP`, `Project2`.`OGRN`, 
`Project2`.`web_password`, `Project2`.`ur_name`, `Project2`.`email`, `Project2`.`icq`, `Project2`.`skype`, `Project2`.`viber`, `Project2`.`telegram`, `Project2`.`site`, 
`Project2`.`whatsapp`, `Project2`.`agent_name`, `Project2`.`agent_surname`, `Project2`.`agent_patronymic`, `Project2`.`agent_phone`, `Project2`.`agent2_name`, `Project2`.`agent2_surname`, 
`Project2`.`agent2_patronymic`, `Project2`.`agent2_phone`, `Project2`.`created`, `Project2`.`balance`, `Project2`.`price_col`, `Project2`.`agent_phone_clean`, 
`Project2`.`agent2_phone_clean`, `Project2`.`repairs`, `Project2`.`token`, `Project2`.`purchases`, `Project2`.`visit_source`, `Project2`.`photo_id`, `Project2`.`C1`, `Project2`.`id1`, 
`Project2`.`phone`, `Project2`.`phone_clean`, `Project2`.`mask`, `Project2`.`customer`, `Project2`.`type1`, `Project2`.`note`
FROM (SELECT
`Limit1`.`id`, `Limit1`.`creator`, `Limit1`.`name`, `Limit1`.`surname`, `Limit1`.`patronymic`, `Limit1`.`agent_phone_mask`, `Limit1`.`agent2_phone_mask`, `Limit1`.`address`, 
`Limit1`.`post_index`, `Limit1`.`passport_num`, `Limit1`.`passport_date`, `Limit1`.`passport_organ`, `Limit1`.`state`, `Limit1`.`type`, `Limit1`.`birthday`, `Limit1`.`memorial`, 
`Limit1`.`notes`, `Limit1`.`is_regular`, `Limit1`.`is_dealer`, `Limit1`.`balance_enable`, `Limit1`.`prefer_cashless`, `Limit1`.`take_long`, `Limit1`.`ignore_calls`, `Limit1`.`is_bad`, 
`Limit1`.`is_realizator`, `Limit1`.`is_agent`, `Limit1`.`visit_source`, `Limit1`.`photo_id`, `Limit1`.`INN`, `Limit1`.`KPP`, `Limit1`.`OGRN`, `Limit1`.`web_password`, `Limit1`.`ur_name`, 
`Limit1`.`email`, `Limit1`.`icq`, `Limit1`.`skype`, `Limit1`.`viber`, `Limit1`.`telegram`, `Limit1`.`site`, `Limit1`.`whatsapp`, `Limit1`.`agent_name`, `Limit1`.`agent_surname`, 
`Limit1`.`agent_patronymic`, `Limit1`.`agent_phone`, `Limit1`.`agent_phone_clean`, `Limit1`.`agent2_name`, `Limit1`.`agent2_surname`, `Limit1`.`agent2_patronymic`, `Limit1`.`agent2_phone`,
 `Limit1`.`agent2_phone_clean`, `Limit1`.`created`, `Limit1`.`balance`, `Limit1`.`price_col`, `Limit1`.`repairs`, `Limit1`.`purchases`, `Limit1`.`token`, `Extent2`.`id` AS `id1`, 
`Extent2`.`phone`, `Extent2`.`phone_clean`, `Extent2`.`mask`, `Extent2`.`customer`, `Extent2`.`type` AS `type1`, `Extent2`.`note`, CASE WHEN (`Extent2`.`id` IS NOT NULL) THEN (1)  ELSE 
(NULL) END AS `C1`
FROM (SELECT
`Extent1`.`id`, `Extent1`.`creator`, `Extent1`.`name`, `Extent1`.`surname`, `Extent1`.`patronymic`, `Extent1`.`agent_phone_mask`, `Extent1`.`agent2_phone_mask`, `Extent1`.`address`, 
`Extent1`.`post_index`, `Extent1`.`passport_num`, `Extent1`.`passport_date`, `Extent1`.`passport_organ`, `Extent1`.`state`, `Extent1`.`type`, `Extent1`.`birthday`, `Extent1`.`memorial`, 
`Extent1`.`notes`, `Extent1`.`is_regular`, `Extent1`.`is_dealer`, `Extent1`.`balance_enable`, `Extent1`.`prefer_cashless`, `Extent1`.`take_long`, `Extent1`.`ignore_calls`, 
`Extent1`.`is_bad`, `Extent1`.`is_realizator`, `Extent1`.`is_agent`, `Extent1`.`visit_source`, `Extent1`.`photo_id`, `Extent1`.`INN`, `Extent1`.`KPP`, `Extent1`.`OGRN`, 
`Extent1`.`web_password`, `Extent1`.`ur_name`, `Extent1`.`email`, `Extent1`.`icq`, `Extent1`.`skype`, `Extent1`.`viber`, `Extent1`.`telegram`, `Extent1`.`site`, `Extent1`.`whatsapp`, 
`Extent1`.`agent_name`, `Extent1`.`agent_surname`, `Extent1`.`agent_patronymic`, `Extent1`.`agent_phone`, `Extent1`.`agent_phone_clean`, `Extent1`.`agent2_name`, 
`Extent1`.`agent2_surname`, `Extent1`.`agent2_patronymic`, `Extent1`.`agent2_phone`, `Extent1`.`agent2_phone_clean`, `Extent1`.`created`, `Extent1`.`balance`, `Extent1`.`price_col`, 
`Extent1`.`repairs`, `Extent1`.`purchases`, `Extent1`.`token`
FROM `clients` AS `Extent1`
 WHERE `Extent1`.`id` = 6251 LIMIT 1) AS `Limit1` LEFT OUTER JOIN `tel` AS `Extent2` ON `Limit1`.`id` = `Extent2`.`customer`) AS `Project2`
 ORDER BY 
`Project2`.`id` ASC, `Project2`.`C1` ASC;

SELECT
`Extent1`.`id`, `Extent1`.`name`, `Extent1`.`position`, `Extent1`.`enabled`, `Extent1`.`fire_on`
FROM `visit_sources` AS `Extent1`
 WHERE `Extent1`.`id` = 1;

SELECT UTC_TIMESTAMP() FROM DUAL;

BEGIN;

UPDATE `workshop` SET `user_lock`=32, `lock_datetime`='2022-03-25 00:12:44.846854' WHERE `id` = 24857;

COMMIT;

SELECT
`GroupBy1`.`A1` AS `C1`
FROM (SELECT
SUM(CASE WHEN (`Project1`.`C2` IS NOT NULL) THEN (`Project1`.`C1`)  ELSE (0) END) AS `A1`
FROM (SELECT
1 AS `X`) AS `SingleRowTable1` LEFT OUTER JOIN (SELECT
`Extent1`.`price` * (`Extent1`.`count`) AS `C1`, 
1 AS `C2`
FROM `works` AS `Extent1`
 WHERE `Extent1`.`repair` = 24857) AS `Project1` ON 1 = 1) AS `GroupBy1`;

SELECT
`GroupBy1`.`A1` AS `C1`
FROM (SELECT
SUM(CASE WHEN (`Project1`.`C2` IS NOT NULL) THEN (`Project1`.`C1`)  ELSE (0) END) AS `A1`
FROM (SELECT
1 AS `X`) AS `SingleRowTable1` LEFT OUTER JOIN (SELECT
(`Extent1`.`count`) * `Extent1`.`price` AS `C1`, 
1 AS `C2`
FROM `store_int_reserve` AS `Extent1`
 WHERE (`Extent1`.`repair_id` = 24857) AND ((2 = `Extent1`.`state`) OR (3 = `Extent1`.`state`))) AS `Project1` ON 1 = 1) AS `GroupBy1`;

SELECT
`Extent1`.`id`, `Extent1`.`item_id`, `Extent1`.`count`, `Extent1`.`created`, `Extent1`.`from_user`, `Extent1`.`to_user`, `Extent1`.`notes`, `Extent1`.`state`, `Extent1`.`repair_id`, 
`Extent1`.`price`, `Extent1`.`sn`, `Extent1`.`work_id`, `Extent1`.`warranty`, `Extent1`.`name`, `Extent1`.`r_lock`, `Extent2`.`id` AS `id1`, `Extent2`.`articul`, `Extent2`.`dealer`, 
`Extent2`.`is_realization`, `Extent2`.`dealer_lock`, `Extent2`.`name` AS `name1`, `Extent2`.`state` AS `state1`, `Extent2`.`category`, `Extent2`.`store`, `Extent2`.`created` AS `created1`,
 `Extent2`.`count` AS `count1`, `Extent2`.`reserved`, `Extent2`.`box`, `Extent2`.`box_name`, `Extent2`.`price_option`, `Extent2`.`currency_rate`, `Extent2`.`in_price`, `Extent2`.`price` AS
 `price1`, `Extent2`.`price2`, `Extent2`.`price3`, `Extent2`.`price4`, `Extent2`.`price5`, `Extent2`.`document`, `Extent2`.`shop_title`, `Extent2`.`shop_description`, `Extent2`.`SN` AS 
`sn1`, `Extent2`.`PN`, `Extent2`.`description`, `Extent2`.`shop_enable`, `Extent2`.`int_barcode`, `Extent2`.`ext_barcode`, `Extent2`.`in_summ`, `Extent2`.`notes` AS `notes1`, 
`Extent2`.`img1`, `Extent2`.`img2`, `Extent2`.`img3`, `Extent2`.`img4`, `Extent2`.`img5`, `Extent2`.`minimum_in_stock`, `Extent2`.`sold`, `Extent2`.`return_percent`, `Extent2`.`warranty` 
AS `warranty1`, `Extent2`.`warranty_dealer`, `Extent2`.`in_count`, `Extent2`.`not_for_sale`, `Extent2`.`part_request`, `Extent2`.`st_state`, `Extent2`.`st_notes`, `Extent2`.`units`, 
`Extent2`.`ge_highlight`, `Extent2`.`Hidden`, `Extent2`.`updated`
FROM `store_int_reserve` AS `Extent1` INNER JOIN `store_items` AS `Extent2` ON `Extent1`.`item_id` = `Extent2`.`id`
 WHERE (`Extent1`.`repair_id` = 24857) AND ((2 = `Extent1`.`state`) OR (3 = `Extent1`.`state`));

SELECT
`Extent1`.`id`, `Extent1`.`client`, `Extent1`.`type`, `Extent1`.`maker`, `Extent1`.`model`, `Extent1`.`serial_number`, `Extent1`.`company`, `Extent1`.`office`, `Extent1`.`start_office`, 
`Extent1`.`manager`, `Extent1`.`current_manager`, `Extent1`.`master`, `Extent1`.`diagnostic_result`, `Extent1`.`in_date`, `Extent1`.`out_date`, `Extent1`.`state`, `Extent1`.`user_lock`, 
`Extent1`.`lock_datetime`, `Extent1`.`express_repair`, `Extent1`.`is_warranty`, `Extent1`.`is_repeat`, `Extent1`.`can_format`, `Extent1`.`print_check`, `Extent1`.`box`, 
`Extent1`.`warranty_label`, `Extent1`.`ext_notes`, `Extent1`.`is_prepaid`, `Extent1`.`prepaid_type`, `Extent1`.`prepaid_summ`, `Extent1`.`prepaid_order`, `Extent1`.`is_pre_agreed`, 
`Extent1`.`pre_agreed_amount`, `Extent1`.`repair_cost`, `Extent1`.`fault`, `Extent1`.`complect`, `Extent1`.`look`, `Extent1`.`thirs_party_sc`, `Extent1`.`last_save`, 
`Extent1`.`last_status_changed`, `Extent1`.`warranty_days`, `Extent1`.`barcode`, `Extent1`.`new_state`, `Extent1`.`is_card_payment`, `Extent1`.`informed_status`, `Extent1`.`parts_cost`, 
`Extent1`.`is_debt`, `Extent1`.`image_ids`, `Extent1`.`color`, `Extent1`.`order_moving`, `Extent1`.`payment_system`, `Extent1`.`early`, `Extent1`.`real_repair_cost`, 
`Extent1`.`issued_msg`, `Extent1`.`reject_reason`, `Extent1`.`ext_early`, `Extent1`.`sms_inform`, `Extent1`.`invoice`, `Extent1`.`cartridge`, `Extent1`.`termsControl`, 
`Extent1`.`vendor_id`, `Extent1`.`quick_repair`, `Extent1`.`Hidden`, `Extent1`.`Title`
FROM `workshop` AS `Extent1`
 WHERE `Extent1`.`id` = 24857;

SELECT UTC_TIMESTAMP() FROM DUAL;

BEGIN;

UPDATE `workshop` SET `out_date`='2022-03-25 00:12:47', `state`=12, `lock_datetime`='2022-03-25 00:12:34', `last_status_changed`='2022-03-25 00:12:47', `reject_reason`='Отказ от ремонта' WHERE `id` = 24857;

INSERT INTO `workshop_issued`(
`repair_id`, 
`employee_id`, 
`created_at`) VALUES (
24857, 
32, 
'2022-03-25 00:12:47');
SELECT
`id`
FROM `workshop_issued`
 WHERE  row_count() > 0 AND `id`=last_insert_id();

COMMIT;

BEGIN;

INSERT INTO `logs`(
`group`, `type`, `arh`, `user`, `created`, `values`, `values_after`, `office`, `client`, `repair`, `item`, `document`, `cash_order`, `part_request`, `notes`) VALUES (
NULL, 3, 0, 32, '2022-03-25 00:12:47.872452', NULL, NULL, 1, NULL, 24857, NULL, NULL, NULL, NULL, 'Заказаз-наряд: 024857, выдан клиенту. Сумма: 0,00 грн., оплата: Наличные');
SELECT
`id`
FROM `logs`
 WHERE  row_count() > 0 AND `id`=last_insert_id();

COMMIT;

SELECT UTC_TIMESTAMP() FROM DUAL;

BEGIN;

INSERT INTO `repair_status_logs`(
`created_at`, `repair_id`, `status_id`, `user_id`, `manager_id`, `master_id`) VALUES (
'2022-03-25 00:12:47', 24857, 7, 32, 34, 34);
SELECT
`id`
FROM `repair_status_logs`
 WHERE  row_count() > 0 AND `id`=last_insert_id();

COMMIT;

SELECT
`Project2`.`id`, `Project2`.`client`, `Project2`.`type`, `Project2`.`maker`, `Project2`.`model`, `Project2`.`serial_number`, `Project2`.`company`, `Project2`.`office`, 
`Project2`.`start_office`, `Project2`.`manager`, `Project2`.`current_manager`, `Project2`.`master`, `Project2`.`diagnostic_result`, `Project2`.`in_date`, `Project2`.`out_date`, 
`Project2`.`state`, `Project2`.`user_lock`, `Project2`.`lock_datetime`, `Project2`.`express_repair`, `Project2`.`is_warranty`, `Project2`.`is_repeat`, `Project2`.`can_format`, 
`Project2`.`print_check`, `Project2`.`box`, `Project2`.`warranty_label`, `Project2`.`ext_notes`, `Project2`.`is_prepaid`, `Project2`.`prepaid_type`, `Project2`.`prepaid_summ`, 
`Project2`.`prepaid_order`, `Project2`.`is_pre_agreed`, `Project2`.`pre_agreed_amount`, `Project2`.`repair_cost`, `Project2`.`fault`, `Project2`.`complect`, `Project2`.`look`, 
`Project2`.`thirs_party_sc`, `Project2`.`last_save`, `Project2`.`last_status_changed`, `Project2`.`warranty_days`, `Project2`.`barcode`, `Project2`.`new_state`, 
`Project2`.`is_card_payment`, `Project2`.`informed_status`, `Project2`.`parts_cost`, `Project2`.`is_debt`, `Project2`.`image_ids`, `Project2`.`color`, `Project2`.`order_moving`, 
`Project2`.`payment_system`, `Project2`.`early`, `Project2`.`real_repair_cost`, `Project2`.`issued_msg`, `Project2`.`reject_reason`, `Project2`.`ext_early`, `Project2`.`sms_inform`, 
`Project2`.`invoice`, `Project2`.`cartridge`, `Project2`.`termsControl`, `Project2`.`vendor_id`, `Project2`.`quick_repair`, `Project2`.`Hidden`, `Project2`.`Title`, `Project2`.`id1`, 
`Project2`.`sip_user_id`, `Project2`.`username`, `Project2`.`name`, `Project2`.`surname`, `Project2`.`patronymic`, `Project2`.`phone`, `Project2`.`phone2`, `Project2`.`phone_mask`, 
`Project2`.`phone2_mask`, `Project2`.`address`, `Project2`.`passport_num`, `Project2`.`passport_date`, `Project2`.`passport_organ`, `Project2`.`state1`, `Project2`.`created`, 
`Project2`.`office1`, `Project2`.`birthday`, `Project2`.`last_login`, `Project2`.`last_activity`, `Project2`.`email`, `Project2`.`sex`, `Project2`.`photo`, `Project2`.`salary_rate`, 
`Project2`.`pay_day`, `Project2`.`pay_day_off`, `Project2`.`pay_repair`, `Project2`.`pay_sale`, `Project2`.`pay_repair_q_sale`, `Project2`.`pay_cartridge_refill`, 
`Project2`.`color_label_ws`, `Project2`.`workspace_mode`, `Project2`.`preview_before_print`, `Project2`.`new_rep_doc_copies`, `Project2`.`auto_refresh_workspace`, 
`Project2`.`refresh_time`, `Project2`.`xls_c1`, `Project2`.`xls_c2`, `Project2`.`xls_c3`, `Project2`.`xls_c4`, `Project2`.`xls_c5`, `Project2`.`xls_c6`, `Project2`.`xls_c7`, 
`Project2`.`xls_c8`, `Project2`.`xls_c9`, `Project2`.`xls_c10`, `Project2`.`xls_c11`, `Project2`.`xls_c12`, `Project2`.`xls_c13`, `Project2`.`xls_c14`, `Project2`.`xls_c15`, 
`Project2`.`display_out`, `Project2`.`display_complete`, `Project2`.`is_bot`, `Project2`.`new_on_top`, `Project2`.`issued_color`, `Project2`.`fields_cfg`, `Project2`.`kkm_pass`, 
`Project2`.`prefer_regular`, `Project2`.`fontsize`, `Project2`.`rowheight`, `Project2`.`animation`, `Project2`.`pay_device_in`, `Project2`.`pay_device_out`, 
`Project2`.`pay_4_sale_in_repair`, `Project2`.`row_color`, `Project2`.`save_state_on_close`, `Project2`.`group_store_items`, `Project2`.`track_activity`, `Project2`.`inn`, 
`Project2`.`inform_comment`, `Project2`.`inform_status`, `Project2`.`kkt`, `Project2`.`pinpad`, `Project2`.`def_office`, `Project2`.`def_store`, `Project2`.`def_item_state`, 
`Project2`.`def_employee`, `Project2`.`def_status`, `Project2`.`def_ws_filter`, `Project2`.`card_on_call`, `Project2`.`ge_highlight_color`, `Project2`.`pay_repair_quick`, 
`Project2`.`advance_disable`, `Project2`.`salary_disable`, `Project2`.`notes`, `Project2`.`id2`, `Project2`.`creator`, `Project2`.`name1`, `Project2`.`surname1`, `Project2`.`patronymic1`, 
`Project2`.`agent_phone_mask`, `Project2`.`agent2_phone_mask`, `Project2`.`address1`, `Project2`.`post_index`, `Project2`.`passport_num1`, `Project2`.`passport_date1`, 
`Project2`.`passport_organ1`, `Project2`.`state2`, `Project2`.`type1`, `Project2`.`birthday1`, `Project2`.`memorial`, `Project2`.`notes1`, `Project2`.`is_regular`, `Project2`.`is_dealer`, 
`Project2`.`balance_enable`, `Project2`.`prefer_cashless`, `Project2`.`take_long`, `Project2`.`ignore_calls`, `Project2`.`is_bad`, `Project2`.`is_realizator`, `Project2`.`is_agent`, 
`Project2`.`inn1`, `Project2`.`KPP`, `Project2`.`OGRN`, `Project2`.`web_password`, `Project2`.`ur_name`, `Project2`.`email1`, `Project2`.`icq`, `Project2`.`skype`, `Project2`.`viber`, 
`Project2`.`telegram`, `Project2`.`site`, `Project2`.`whatsapp`, `Project2`.`agent_name`, `Project2`.`agent_surname`, `Project2`.`agent_patronymic`, `Project2`.`agent_phone`, 
`Project2`.`agent2_name`, `Project2`.`agent2_surname`, `Project2`.`agent2_patronymic`, `Project2`.`agent2_phone`, `Project2`.`created1`, `Project2`.`balance`, `Project2`.`price_col`, 
`Project2`.`agent_phone_clean`, `Project2`.`agent2_phone_clean`, `Project2`.`repairs`, `Project2`.`token`, `Project2`.`purchases`, `Project2`.`visit_source`, `Project2`.`photo_id`, 
`Project2`.`id3`, `Project2`.`num`, `Project2`.`created2`, `Project2`.`user`, `Project2`.`seller`, `Project2`.`customer`, `Project2`.`paid`, `Project2`.`tax`, `Project2`.`summ`, 
`Project2`.`total`, `Project2`.`state3`, `Project2`.`office2`, `Project2`.`type2`, `Project2`.`notes2`, `Project2`.`id4`, `Project2`.`refill`, `Project2`.`chip`, `Project2`.`opc_drum`, 
`Project2`.`c_blade`, `Project2`.`card_id`, `Project2`.`id5`, `Project2`.`name2`, `Project2`.`maker1`, `Project2`.`full_weight`, `Project2`.`toner_weight`, `Project2`.`resource`, 
`Project2`.`created3`, `Project2`.`user1`, `Project2`.`notes3`, `Project2`.`photo1`, `Project2`.`color1`, `Project2`.`archive`, `Project2`.`C1`, `Project2`.`id6`, `Project2`.`field_id`, 
`Project2`.`repair_id`, `Project2`.`value`, `Project2`.`item_id`, `Project2`.`id7`, `Project2`.`name3`, `Project2`.`type3`, `Project2`.`def_values`, `Project2`.`required`, 
`Project2`.`printable`, `Project2`.`archive1`, `Project2`.`_f`, `Project2`.`updated_at`, `Project2`.`devices`, `Project2`.`categories`
FROM (SELECT
`Limit1`.`id`, `Limit1`.`Hidden`, `Limit1`.`Title`, `Limit1`.`client`, `Limit1`.`type`, `Limit1`.`maker`, `Limit1`.`model`, `Limit1`.`serial_number`, `Limit1`.`company`, `Limit1`.`office`,
 `Limit1`.`start_office`, `Limit1`.`manager`, `Limit1`.`current_manager`, `Limit1`.`master`, `Limit1`.`diagnostic_result`, `Limit1`.`in_date`, `Limit1`.`out_date`, `Limit1`.`state`, 
`Limit1`.`new_state`, `Limit1`.`user_lock`, `Limit1`.`lock_datetime`, `Limit1`.`express_repair`, `Limit1`.`quick_repair`, `Limit1`.`is_warranty`, `Limit1`.`is_repeat`, 
`Limit1`.`payment_system`, `Limit1`.`is_card_payment`, `Limit1`.`can_format`, `Limit1`.`print_check`, `Limit1`.`box`, `Limit1`.`warranty_label`, `Limit1`.`ext_notes`, 
`Limit1`.`is_prepaid`, `Limit1`.`prepaid_type`, `Limit1`.`prepaid_summ`, `Limit1`.`prepaid_order`, `Limit1`.`is_pre_agreed`, `Limit1`.`is_debt`, `Limit1`.`pre_agreed_amount`, 
`Limit1`.`repair_cost`, `Limit1`.`real_repair_cost`, `Limit1`.`parts_cost`, `Limit1`.`fault`, `Limit1`.`complect`, `Limit1`.`look`, `Limit1`.`thirs_party_sc`, `Limit1`.`last_save`, 
`Limit1`.`last_status_changed`, `Limit1`.`warranty_days`, `Limit1`.`barcode`, `Limit1`.`reject_reason`, `Limit1`.`informed_status`, `Limit1`.`image_ids`, `Limit1`.`color`, 
`Limit1`.`order_moving`, `Limit1`.`early`, `Limit1`.`ext_early`, `Limit1`.`issued_msg`, `Limit1`.`sms_inform`, `Limit1`.`invoice`, `Limit1`.`cartridge`, `Limit1`.`vendor_id`, 
`Limit1`.`termsControl`, `Limit1`.`id1`, `Limit1`.`sip_user_id`, `Limit1`.`username`, `Limit1`.`name`, `Limit1`.`surname`, `Limit1`.`patronymic`, `Limit1`.`phone`, `Limit1`.`phone2`, 
`Limit1`.`phone_mask`, `Limit1`.`phone2_mask`, `Limit1`.`address`, `Limit1`.`passport_num`, `Limit1`.`passport_date`, `Limit1`.`passport_organ`, `Limit1`.`state1`, `Limit1`.`created`, 
`Limit1`.`office1`, `Limit1`.`birthday`, `Limit1`.`def_office`, `Limit1`.`def_store`, `Limit1`.`def_item_state`, `Limit1`.`def_employee`, `Limit1`.`def_status`, `Limit1`.`def_ws_filter`, 
`Limit1`.`last_login`, `Limit1`.`last_activity`, `Limit1`.`email`, `Limit1`.`sex`, `Limit1`.`photo`, `Limit1`.`salary_rate`, `Limit1`.`pay_day`, `Limit1`.`pay_day_off`, 
`Limit1`.`pay_repair`, `Limit1`.`pay_repair_quick`, `Limit1`.`pay_sale`, `Limit1`.`pay_repair_q_sale`, `Limit1`.`pay_cartridge_refill`, `Limit1`.`pay_device_in`, `Limit1`.`pay_device_out`,
 `Limit1`.`pay_4_sale_in_repair`, `Limit1`.`row_color`, `Limit1`.`ge_highlight_color`, `Limit1`.`color_label_ws`, `Limit1`.`workspace_mode`, `Limit1`.`preview_before_print`, 
`Limit1`.`new_rep_doc_copies`, `Limit1`.`auto_refresh_workspace`, `Limit1`.`refresh_time`, `Limit1`.`xls_c1`, `Limit1`.`xls_c2`, `Limit1`.`xls_c3`, `Limit1`.`xls_c4`, `Limit1`.`xls_c5`, 
`Limit1`.`xls_c6`, `Limit1`.`xls_c7`, `Limit1`.`xls_c8`, `Limit1`.`xls_c9`, `Limit1`.`xls_c10`, `Limit1`.`xls_c11`, `Limit1`.`xls_c12`, `Limit1`.`xls_c13`, `Limit1`.`xls_c14`, 
`Limit1`.`xls_c15`, `Limit1`.`display_out`, `Limit1`.`display_complete`, `Limit1`.`is_bot`, `Limit1`.`new_on_top`, `Limit1`.`issued_color`, `Limit1`.`kkm_pass`, `Limit1`.`prefer_regular`, 
`Limit1`.`fontsize`, `Limit1`.`rowheight`, `Limit1`.`animation`, `Limit1`.`fields_cfg`, `Limit1`.`save_state_on_close`, `Limit1`.`group_store_items`, `Limit1`.`track_activity`, 
`Limit1`.`card_on_call`, `Limit1`.`inn`, `Limit1`.`inform_comment`, `Limit1`.`inform_status`, `Limit1`.`kkt`, `Limit1`.`pinpad`, `Limit1`.`advance_disable`, `Limit1`.`salary_disable`, 
`Limit1`.`notes`, `Limit1`.`id2`, `Limit1`.`creator`, `Limit1`.`name1`, `Limit1`.`surname1`, `Limit1`.`patronymic1`, `Limit1`.`agent_phone_mask`, `Limit1`.`agent2_phone_mask`, 
`Limit1`.`address1`, `Limit1`.`post_index`, `Limit1`.`passport_num1`, `Limit1`.`passport_date1`, `Limit1`.`passport_organ1`, `Limit1`.`state2`, `Limit1`.`type1`, `Limit1`.`birthday1`, 
`Limit1`.`memorial`, `Limit1`.`notes1`, `Limit1`.`is_regular`, `Limit1`.`is_dealer`, `Limit1`.`balance_enable`, `Limit1`.`prefer_cashless`, `Limit1`.`take_long`, `Limit1`.`ignore_calls`, 
`Limit1`.`is_bad`, `Limit1`.`is_realizator`, `Limit1`.`is_agent`, `Limit1`.`visit_source`, `Limit1`.`photo_id`, `Limit1`.`inn1`, `Limit1`.`KPP`, `Limit1`.`OGRN`, `Limit1`.`web_password`, 
`Limit1`.`ur_name`, `Limit1`.`email1`, `Limit1`.`icq`, `Limit1`.`skype`, `Limit1`.`viber`, `Limit1`.`telegram`, `Limit1`.`site`, `Limit1`.`whatsapp`, `Limit1`.`agent_name`, 
`Limit1`.`agent_surname`, `Limit1`.`agent_patronymic`, `Limit1`.`agent_phone`, `Limit1`.`agent_phone_clean`, `Limit1`.`agent2_name`, `Limit1`.`agent2_surname`, 
`Limit1`.`agent2_patronymic`, `Limit1`.`agent2_phone`, `Limit1`.`agent2_phone_clean`, `Limit1`.`created1`, `Limit1`.`balance`, `Limit1`.`price_col`, `Limit1`.`repairs`, 
`Limit1`.`purchases`, `Limit1`.`token`, `Limit1`.`id3`, `Limit1`.`num`, `Limit1`.`created2`, `Limit1`.`user`, `Limit1`.`seller`, `Limit1`.`customer`, `Limit1`.`paid`, `Limit1`.`tax`, 
`Limit1`.`summ`, `Limit1`.`total`, `Limit1`.`state3`, `Limit1`.`office2`, `Limit1`.`type2`, `Limit1`.`notes2`, `Limit1`.`id4`, `Limit1`.`refill`, `Limit1`.`chip`, `Limit1`.`opc_drum`, 
`Limit1`.`c_blade`, `Limit1`.`card_id`, `Limit1`.`id5`, `Limit1`.`name2`, `Limit1`.`maker1`, `Limit1`.`full_weight`, `Limit1`.`toner_weight`, `Limit1`.`resource`, `Limit1`.`created3`, 
`Limit1`.`user1`, `Limit1`.`notes3`, `Limit1`.`photo1`, `Limit1`.`color1`, `Limit1`.`archive`, `Join6`.`id` AS `id6`, `Join6`.`field_id`, `Join6`.`repair_id`, `Join6`.`item_id`, 
`Join6`.`value`, `Join6`.`ID1` AS `id7`, `Join6`.`_f`, `Join6`.`name` AS `name3`, `Join6`.`type` AS `type3`, `Join6`.`def_values`, `Join6`.`required`, `Join6`.`printable`, 
`Join6`.`devices`, `Join6`.`categories`, `Join6`.`updated_at`, `Join6`.`archive` AS `archive1`, CASE WHEN (`Join6`.`id` IS NOT NULL) THEN (1)  ELSE (NULL) END AS `C1`
FROM (SELECT
`Extent1`.`id`, `Extent1`.`Hidden`, `Extent1`.`Title`, `Extent1`.`client`, `Extent1`.`type`, `Extent1`.`maker`, `Extent1`.`model`, `Extent1`.`serial_number`, `Extent1`.`company`, 
`Extent1`.`office`, `Extent1`.`start_office`, `Extent1`.`manager`, `Extent1`.`current_manager`, `Extent1`.`master`, `Extent1`.`diagnostic_result`, `Extent1`.`in_date`, 
`Extent1`.`out_date`, `Extent1`.`state`, `Extent1`.`new_state`, `Extent1`.`user_lock`, `Extent1`.`lock_datetime`, `Extent1`.`express_repair`, `Extent1`.`quick_repair`, 
`Extent1`.`is_warranty`, `Extent1`.`is_repeat`, `Extent1`.`payment_system`, `Extent1`.`is_card_payment`, `Extent1`.`can_format`, `Extent1`.`print_check`, `Extent1`.`box`, 
`Extent1`.`warranty_label`, `Extent1`.`ext_notes`, `Extent1`.`is_prepaid`, `Extent1`.`prepaid_type`, `Extent1`.`prepaid_summ`, `Extent1`.`prepaid_order`, `Extent1`.`is_pre_agreed`, 
`Extent1`.`is_debt`, `Extent1`.`pre_agreed_amount`, `Extent1`.`repair_cost`, `Extent1`.`real_repair_cost`, `Extent1`.`parts_cost`, `Extent1`.`fault`, `Extent1`.`complect`, 
`Extent1`.`look`, `Extent1`.`thirs_party_sc`, `Extent1`.`last_save`, `Extent1`.`last_status_changed`, `Extent1`.`warranty_days`, `Extent1`.`barcode`, `Extent1`.`reject_reason`, 
`Extent1`.`informed_status`, `Extent1`.`image_ids`, `Extent1`.`color`, `Extent1`.`order_moving`, `Extent1`.`early`, `Extent1`.`ext_early`, `Extent1`.`issued_msg`, `Extent1`.`sms_inform`, 
`Extent1`.`invoice`, `Extent1`.`cartridge`, `Extent1`.`vendor_id`, `Extent1`.`termsControl`, `Extent2`.`id` AS `id1`, `Extent2`.`sip_user_id`, `Extent2`.`username`, `Extent2`.`name`, 
`Extent2`.`surname`, `Extent2`.`patronymic`, `Extent2`.`phone`, `Extent2`.`phone2`, `Extent2`.`phone_mask`, `Extent2`.`phone2_mask`, `Extent2`.`address`, `Extent2`.`passport_num`, 
`Extent2`.`passport_date`, `Extent2`.`passport_organ`, `Extent2`.`state` AS `state1`, `Extent2`.`created`, `Extent2`.`office` AS `office1`, `Extent2`.`birthday`, `Extent2`.`def_office`, 
`Extent2`.`def_store`, `Extent2`.`def_item_state`, `Extent2`.`def_employee`, `Extent2`.`def_status`, `Extent2`.`def_ws_filter`, `Extent2`.`last_login`, `Extent2`.`last_activity`, 
`Extent2`.`email`, `Extent2`.`sex`, `Extent2`.`photo`, `Extent2`.`salary_rate`, `Extent2`.`pay_day`, `Extent2`.`pay_day_off`, `Extent2`.`pay_repair`, `Extent2`.`pay_repair_quick`, 
`Extent2`.`pay_sale`, `Extent2`.`pay_repair_q_sale`, `Extent2`.`pay_cartridge_refill`, `Extent2`.`pay_device_in`, `Extent2`.`pay_device_out`, `Extent2`.`pay_4_sale_in_repair`, 
`Extent2`.`row_color`, `Extent2`.`ge_highlight_color`, `Extent2`.`color_label_ws`, `Extent2`.`workspace_mode`, `Extent2`.`preview_before_print`, `Extent2`.`new_rep_doc_copies`, 
`Extent2`.`auto_refresh_workspace`, `Extent2`.`refresh_time`, `Extent2`.`xls_c1`, `Extent2`.`xls_c2`, `Extent2`.`xls_c3`, `Extent2`.`xls_c4`, `Extent2`.`xls_c5`, `Extent2`.`xls_c6`, 
`Extent2`.`xls_c7`, `Extent2`.`xls_c8`, `Extent2`.`xls_c9`, `Extent2`.`xls_c10`, `Extent2`.`xls_c11`, `Extent2`.`xls_c12`, `Extent2`.`xls_c13`, `Extent2`.`xls_c14`, `Extent2`.`xls_c15`, 
`Extent2`.`display_out`, `Extent2`.`display_complete`, `Extent2`.`is_bot`, `Extent2`.`new_on_top`, `Extent2`.`issued_color`, `Extent2`.`kkm_pass`, `Extent2`.`prefer_regular`, 
`Extent2`.`fontsize`, `Extent2`.`rowheight`, `Extent2`.`animation`, `Extent2`.`fields_cfg`, `Extent2`.`save_state_on_close`, `Extent2`.`group_store_items`, `Extent2`.`track_activity`, 
`Extent2`.`card_on_call`, `Extent2`.`inn`, `Extent2`.`inform_comment`, `Extent2`.`inform_status`, `Extent2`.`kkt`, `Extent2`.`pinpad`, `Extent2`.`advance_disable`, 
`Extent2`.`salary_disable`, `Extent2`.`notes`, `Extent3`.`id` AS `id2`, `Extent3`.`creator`, `Extent3`.`name` AS `name1`, `Extent3`.`surname` AS `surname1`, `Extent3`.`patronymic` AS 
`patronymic1`, `Extent3`.`agent_phone_mask`, `Extent3`.`agent2_phone_mask`, `Extent3`.`address` AS `address1`, `Extent3`.`post_index`, `Extent3`.`passport_num` AS `passport_num1`, 
`Extent3`.`passport_date` AS `passport_date1`, `Extent3`.`passport_organ` AS `passport_organ1`, `Extent3`.`state` AS `state2`, `Extent3`.`type` AS `type1`, `Extent3`.`birthday` AS 
`birthday1`, `Extent3`.`memorial`, `Extent3`.`notes` AS `notes1`, `Extent3`.`is_regular`, `Extent3`.`is_dealer`, `Extent3`.`balance_enable`, `Extent3`.`prefer_cashless`, 
`Extent3`.`take_long`, `Extent3`.`ignore_calls`, `Extent3`.`is_bad`, `Extent3`.`is_realizator`, `Extent3`.`is_agent`, `Extent3`.`visit_source`, `Extent3`.`photo_id`, `Extent3`.`INN` AS 
`inn1`, `Extent3`.`KPP`, `Extent3`.`OGRN`, `Extent3`.`web_password`, `Extent3`.`ur_name`, `Extent3`.`email` AS `email1`, `Extent3`.`icq`, `Extent3`.`skype`, `Extent3`.`viber`, 
`Extent3`.`telegram`, `Extent3`.`site`, `Extent3`.`whatsapp`, `Extent3`.`agent_name`, `Extent3`.`agent_surname`, `Extent3`.`agent_patronymic`, `Extent3`.`agent_phone`, 
`Extent3`.`agent_phone_clean`, `Extent3`.`agent2_name`, `Extent3`.`agent2_surname`, `Extent3`.`agent2_patronymic`, `Extent3`.`agent2_phone`, `Extent3`.`agent2_phone_clean`, 
`Extent3`.`created` AS `created1`, `Extent3`.`balance`, `Extent3`.`price_col`, `Extent3`.`repairs`, `Extent3`.`purchases`, `Extent3`.`token`, `Extent4`.`id` AS `id3`, `Extent4`.`num`, 
`Extent4`.`created` AS `created2`, `Extent4`.`user`, `Extent4`.`seller`, `Extent4`.`customer`, `Extent4`.`paid`, `Extent4`.`tax`, `Extent4`.`summ`, `Extent4`.`total`, `Extent4`.`state` AS 
`state3`, `Extent4`.`office` AS `office2`, `Extent4`.`type` AS `type2`, `Extent4`.`notes` AS `notes2`, `Extent5`.`id` AS `id4`, `Extent5`.`refill`, `Extent5`.`chip`, `Extent5`.`opc_drum`, 
`Extent5`.`c_blade`, `Extent5`.`card_id`, `Extent6`.`id` AS `id5`, `Extent6`.`name` AS `name2`, `Extent6`.`maker` AS `maker1`, `Extent6`.`full_weight`, `Extent6`.`toner_weight`, 
`Extent6`.`resource`, `Extent6`.`created` AS `created3`, `Extent6`.`user` AS `user1`, `Extent6`.`notes` AS `notes3`, `Extent6`.`photo` AS `photo1`, `Extent6`.`color` AS `color1`, 
`Extent6`.`archive`
FROM `workshop` AS `Extent1` LEFT OUTER JOIN `users` AS `Extent2` ON `Extent1`.`user_lock` = `Extent2`.`id` INNER JOIN `clients` AS `Extent3` ON `Extent1`.`client` = `Extent3`.`id` LEFT OUTER JOIN `invoice` AS `Extent4` ON `Extent1`.`invoice` = `Extent4`.`id` LEFT OUTER JOIN `c_workshop` AS `Extent5` ON `Extent1`.`cartridge` = `Extent5`.`id` LEFT OUTER JOIN `cartridge_cards` AS `Extent6` ON `Extent5`.`card_id` = `Extent6`.`id`
 WHERE `Extent1`.`id` = 24857 LIMIT 2) AS `Limit1` LEFT OUTER JOIN (SELECT
`Extent7`.`id`, `Extent7`.`field_id`, `Extent7`.`repair_id`, `Extent7`.`item_id`, `Extent7`.`value`, `Extent8`.`id` AS `ID1`, `Extent8`.`_f`, `Extent8`.`name`, `Extent8`.`type`, `Extent8`.`def_values`, `Extent8`.`required`, `Extent8`.`printable`, `Extent8`.`devices`, `Extent8`.`categories`, `Extent8`.`updated_at`, `Extent8`.`archive`
FROM `field_values` AS `Extent7` INNER JOIN `fields` AS `Extent8` ON `Extent7`.`field_id` = `Extent8`.`id`) AS `Join6` ON `Limit1`.`id` = `Join6`.`repair_id`) AS `Project2`
 ORDER BY 
`Project2`.`id` ASC, `Project2`.`id1` ASC, `Project2`.`id2` ASC, `Project2`.`id3` ASC, `Project2`.`id4` ASC, `Project2`.`id5` ASC, `Project2`.`C1` ASC;

SELECT UTC_TIMESTAMP() FROM DUAL;


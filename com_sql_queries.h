#ifndef COM_SQL_QUERIES_H
#define COM_SQL_QUERIES_H

#include "appver.h"

#define QUERY_SEL_CLIENT(id)                QString("SELECT  `id`,  `name`,  `surname`,  `patronymic`,  `agent_phone_mask`,  `agent2_phone_mask`,  `address`,  `post_index`,  `type`,  `memorial`,  `notes`,  `prefer_cashless`,  `visit_source`,  `ur_name`, `short_name`,  `email`,  `balance_enable`,  `price_col`,  `repairs`,  `purchases`, `creator`,  `post_index`,  `passport_num`,  `passport_date`,  `passport_organ`,  `state`,  `birthday`,  `is_regular`,  `is_dealer`,  `take_long`,  `ignore_calls`,  `is_bad`,  `is_realizator`,  `is_agent`,  `photo_id`,  `INN`,  `KPP`,  `OGRN`,  `web_password`,  `icq`,  `skype`,  `viber`,  `telegram`,  `site`,  `whatsapp`,  `agent_name`,  `agent_surname`,  `agent_patronymic`,  `agent_phone`,  `agent_phone_clean`,  `agent2_name`,  `agent2_surname`,  `agent2_patronymic`,  `agent2_phone`,  `agent2_phone_clean`,  `created`,  `token` FROM `clients` WHERE `id` = %1 AND `state` = 1").arg((id))
#define QUERY_SEL_BALANCE(id)               QString("SELECT ROUND(`balance`, 2) AS 'balance' FROM clients WHERE `id` = %1;").arg((id))
#define QUERY_SEL_CLIENT_AD_TYPES           QString("SELECT `name`, `id` FROM visit_sources WHERE `enabled` ORDER BY `position` ASC;")
#define QUERY_SEL_CLIENT_MATCH_STATIC       QString(\
                                                "SELECT                                                                      \n"\
                                                "  t1.`id`,                                                                  \n"\
                                                "  IF(t1.`type`,                                                             \n"\
                                                "    IF(LENGTH(TRIM(t1.`ur_name`)), t1.`ur_name`, t1.`name`),                \n"\
                                                "    CONCAT_WS(' ', t1.`surname`, t1.`name`, t1.`patronymic`)) AS 'FIO',     \n"\
                                                "  t1.`balance`,                                                             \n"\
                                                "  t1.`repairs`,                                                             \n"\
                                                "  t1.`purchases`,                                                           \n"\
                                                "  IF(t1.`type` = 1, 'Ю', '') AS 'type',                                     \n"\
                                                "  GROUP_CONCAT(                                                             \n"\
                                                "    IFNULL(t2.`phone`, '') ORDER BY t2.`type` DESC,                         \n"\
                                                "    t2.`id` DESC                                                            \n"\
                                                "    SEPARATOR '\r\n')  AS 'phone',                                          \n"\
                                                "  t1.`short_name`                                                           \n"\
                                                "FROM                                                                        \n"\
                                                "  `clients` AS t1                                                           \n"\
                                                "  LEFT JOIN `tel` AS t2                                                     \n"\
                                                "    ON t1.`id` = t2.`customer`                                              \n")
#define QUERY_SEL_CLIENT_PHONES(id)         QString("SELECT `phone`, `mask`, `type`, `phone_clean`, `note`, `viber`, `telegram`, `whatsapp`, `notify`, `id`, `customer` FROM `tel` WHERE `customer` = " + QString::number((id)) + " ORDER BY `type` DESC, `id` DESC")
#define QUERY_SEL_USER_STATE(username)      QString("SELECT `id` FROM `users` WHERE `username` = '%1' AND `state` = 1 AND `is_bot` = 0 LIMIT 1;").arg((username))
#define QUERY_SEL_USER_DATA(username)       QString("SELECT `users`.`id`, GROUP_CONCAT(`role_id`) AS 'roles', `name`,  `surname`,  `patronymic`, CONCAT(IF(users.`surname` IS NULL OR users.`surname` = '', '', CONCAT(users.`surname`, ' ')), IF(users.`name` IS NULL OR users.`name` = '', '', CONCAT(LEFT(users.`name`, 1), '. ')), IF(users.`patronymic` IS NULL OR users.`patronymic` = '', '', CONCAT(LEFT(users.`patronymic`, 1), '.'))) AS 'fio', `office`,  `def_office`,  `def_store`,  `def_item_state`,  `def_employee`,  `def_status`,  `def_ws_filter`,  `salary_rate`,  `pay_day`,  `pay_day_off`,  `pay_repair`,  `pay_repair_quick`,  `pay_sale`,  `pay_repair_q_sale`,  `pay_cartridge_refill`,  `pay_device_in`,  `pay_device_out`,  `pay_4_sale_in_repair`,  `row_color`,  `ge_highlight_color`,  `color_label_ws`,  `workspace_mode`,  `preview_before_print`,  `new_rep_doc_copies`,  `auto_refresh_workspace`,  `refresh_time`,  `xls_c1`,  `xls_c2`,  `xls_c3`,  `xls_c4`,  `xls_c5`,  `xls_c6`,  `xls_c7`,  `xls_c8`,  `xls_c9`,  `xls_c10`,  `xls_c11`,  `xls_c12`,  `xls_c13`,  `xls_c14`,  `xls_c15`,  `display_out`,  `display_complete`,  `new_on_top`,  `issued_color`, `fields_cfg`,  `save_state_on_close`,  `group_store_items`,  `track_activity`,  `card_on_call`,  `inn`,  `inform_comment`,  `inform_status`,  `kkt`,  `pinpad`,  `advance_disable`,  `salary_disable`,  `kkm_pass`,  `prefer_regular`,  `fontsize`,  `rowheight`,  `animation`, `username` FROM `users` LEFT JOIN `roles_users` ON `users`.`id` = `roles_users`.`user_id` WHERE `username` = '%1' GROUP BY `users`.`id`  LIMIT 1;").arg((username))
#define QUERY_SEL_USER_DATA2(id)            QString("SELECT `users`.`id`, `sip_user_id`, `client_user_id`, `username`, `name`, `surname`, `patronymic`, `phone`, `phone2`, `phone_mask`, `phone2_mask`, `address`, `passport_num`, `passport_date`, `passport_organ`, `state`, `created`, `office`, `birthday`, `def_office`, `def_store`, `def_item_state`, `def_employee`, `def_status`, `def_ws_filter`, `last_login`, `last_activity`, `email`, `sex`,  `photo`, `salary_rate`, `pay_day`, `pay_day_off`, `pay_repair`, `pay_repair_quick`, `pay_sale`, `pay_repair_q_sale`, `pay_cartridge_refill`, `pay_device_in`, `pay_device_out`, `pay_4_sale_in_repair`, `row_color`, `ge_highlight_color`, `color_label_ws`, `workspace_mode`, `preview_before_print`, `new_rep_doc_copies`, `auto_refresh_workspace`, `refresh_time`, `xls_c1`, `xls_c2`, `xls_c3`, `xls_c4`, `xls_c5`, `xls_c6`, `xls_c7`, `xls_c8`, `xls_c9`, `xls_c10`, `xls_c11`, `xls_c12`, `xls_c13`, `xls_c14`, `xls_c15`, `display_out`, `display_complete`, `is_bot`, `new_on_top`, `issued_color`,  `fields_cfg`, `save_state_on_close`, `group_store_items`, `track_activity`, `card_on_call`, `inn`, `inform_comment`, `inform_status`, `kkt`, `pinpad`, `advance_disable`, `salary_disable`, `notes`, `signature`, `kkm_pass`, `prefer_regular`, `fontsize`, `rowheight`, `animation` FROM `users` LEFT JOIN `roles_users` ON `users`.`id` = `roles_users`.`user_id` WHERE `users`.`id` = '%1' GROUP BY `users`.`id` LIMIT 1;").arg((id))
#define QUERY_SEL_ASC_SCHEMA_VER            QString("SELECT `scriptname` FROM schemaversions WHERE `scriptname` LIKE 'ASC%' ORDER BY LPAD(REPLACE(`scriptname`, 'ASC.Scripts.Script', ''), 6, '0') DESC;")
#define QUERY_SEL_APP_VER                   QString("SELECT `version_snap` FROM `config` WHERE `id` = 1;")
#define QUERY_SEL_SCHEMA_VER                QString("SELECT REPLACE(`scriptname`, 'SNAP.schema-updates.', '') FROM schemaversions WHERE `scriptname` LIKE 'SNAP%' ORDER BY LPAD(REPLACE(`scriptname`, 'SNAP.schema-updates.script', ''), 6, '0') ASC;")
#define QUERY_SEL_UPDATE_CHANNEL            QString("SELECT `value` FROM `settings` WHERE `name` = 'update_channel';")
#define QUERY_SEL_ACTIVE_USERS(db)          QString("SELECT GROUP_CONCAT(DISTINCT `USER`) AS 'users' FROM `information_schema`.`PROCESSLIST` WHERE `DB` = '%1' AND `USER` IN (SELECT `username` FROM `%1`.`users` WHERE `is_bot` <> 1) AND `USER` NOT IN ('root', SUBSTRING_INDEX(USER(), '@', 1)) GROUP BY `DB`;")\
                                                .arg((db))
#define QUERY_SEL_GLOB_WAIT_TIMEOUT         QString("SELECT `VARIABLE_VALUE` FROM performance_schema.global_variables WHERE `VARIABLE_NAME` LIKE 'wait_timeout';")
#define QUERY_SEL_ACTIVE_USERS2(timeout)    QString("SELECT GROUP_CONCAT(`username`) FROM users WHERE `last_activity` > DATE_SUB(NOW(), INTERVAL %1 SECOND);").arg(timeout)
#define QUERY_SEL_DOC_TEMPL_CHECKSUM(doc)   QString("SELECT HEX(`checksum`) AS 'checksum' FROM `doc_templates_snap` WHERE `name` = '%1' LIMIT 1;").arg(doc)
#define QUERY_SEL_DOC_TEMPL_DATA(doc)       QString("SELECT `data` FROM `doc_templates_snap` WHERE `name` = '%1' LIMIT 1;").arg(doc)
#define QUERY_SEL_PERMISSIONS(roles)        QString("SELECT `permission_id` FROM `permissions_roles` WHERE `role_id` IN (%1)").arg((roles))
#define QUERY_SEL_COMMON_SETTINGS           QString("SELECT  `id`,  `default_works_warranty`,  `default_items_warranty`,  `currency`,  `configcol`,  `email_config`,  `qs_print_pko`,  `qs_print_rko`,  `qs_print_pn`,  `qs_print_rn`,  `print_rep_stickers`,  `rep_stickers_copy`,  `rep_auto_company`,  `config_modified`,  `close_clients`,  `aster_host`,  `aster_port`,  `aster_login`,  `aster_password`,  `aster_web_port`,  `time_zone`,  `realizator_enable`,  `online_store`,  `online_store_api`,  `online_store_key`,  `classic_kassa`,  `default_items_used_warranty`,  `default_items_rep_warranty`,  `default_items_razb_warranty`,  `default_items_other_warranty`,  `images_width`,  `item_img_limit`,  `rep_img_limit`,  `default_reserve_days`,  `it_vis_opt`,  `it_vis_opt2`,  `it_vis_opt3`,  `it_vis_rozn`,  `it_vis_price_for_sc`,  `give2user_time`,  `diag_accept_time`,  `salary_day`,  `parts_included`,  `visit_source_force`,  `is_patronymic_required`,  `is_photo_required`,  `is_photo_out_req`,  `is_sn_req`,  `is_cartridge_sn_req`,  `is_reason_req`,  `address_required`,  `email_required`,  `phone_required`,  `diag_required`,  `is_master_set_on_new`,  `print_warranty`,  `print_works`,  `print_diagnostic`,  `print_reject`,  `print_check`,  `rko_on_pn`,  `card_close_time`,  `phone_mask1`,  `phone_mask2`,  `auto_switch_layout`,  `rep_price_by_manager`,  `video_codec`,  `print_new_repair_report`,  `statuses`,  `sms_config`,  `debt_rep_2_salary`,  `exchange_type`,  `exchange_source`,  `exchange_rate`,  `history_by_sn`,  `clients_are_dealers`,  `regular_customer_step`,  `backup_enable`,  `backup_images`,  `backup_time`,  `manual_maker`,  `TimeOfWork`,  `cartridge_enable`,  `vw_enable`,  `termsControl`,  `version` FROM `config` WHERE `id` = 1;")
#define QUERY_SEL_COMMON_SETTINGS2          QString("SELECT  `name`,  `value` FROM `settings`")
#define QUERY_SEL_DOC_TEMPLATES_LIST        QString("SELECT  `description`, `name`, `id`, `checksum` FROM `doc_templates_snap`;")
#define QUERY_SEL_COMPANIES                 QString("SELECT  `name`,  `id`,  `type`,  `inn`,  `kpp`,  `ogrn`,  `ur_address`,  `site`,  `email`,  `logo`,  `banks`,  `is_default`,  `status`,  `director`,  `accountant`,  `tax_form`,  `description` FROM `companies` WHERE `status` = 1;")
#define QUERY_SEL_OFFICES(id)               QString("SELECT  t1.`name`, CONCAT(t1.`name`, ' (', t2.`name`, ')') AS 'name_full',  t1.`id`,  t1.`state`,  `address`,  `phone`,  t1.`logo` AS 'logo',  t2.`logo` AS 'companyLogo',  `administrator`,  `created`,  `phone2`,  `default_company`,  `card_payment`,  `use_boxes`,  `paint_repairs`,  `warranty_sn` FROM `offices` AS t1 LEFT JOIN `companies` AS t2 ON t1.`default_company` = t2.`id` WHERE t1.`state` = 1;")
#define QUERY_SEL_PHONE_TYPES               QString("SELECT `name`, `id`, `mask` FROM `tel_types` WHERE `enabled` = 1 ORDER BY `order` ASC;")
#define QUERY_SEL_WAREHOUSES(office)        QString("SELECT `name`, `id`, `type`, `description`, `sub_type`, `it_vis_pn`, `it_vis_notes`, `it_vis_description`, `it_vis_sn`, `it_vis_barcode`, `it_vis_photo`, `it_vis_warranty`, `it_vis_warranty_dealer`, `active`FROM `stores` WHERE `office` = %1;").arg((office))
#define QUERY_SEL_ALL_USERS                 QString("SELECT `username`, `id`,  `name`,  `surname`,  `patronymic`, `office`, `inform_comment`, `inform_status` FROM `users` WHERE `is_bot` = 0;")
#define QUERY_SEL_USERS                     QString("SELECT `username`, `id`,  `name`,  `surname`,  `patronymic`, `office`, `inform_comment`, `inform_status` FROM `users` WHERE `state` = 1 AND  `is_bot` = 0;")
#define QUERY_SEL_USERS_SALARY_TAXES        QString("SELECT `username`, `id`,  `salary_rate`,  `pay_day`,  `pay_day_off`,  `pay_repair`,  `pay_repair_quick`,  `pay_sale`,  `pay_repair_q_sale`,  `pay_cartridge_refill`,  `pay_device_in`,  `pay_device_out`,  `pay_4_sale_in_repair` FROM `users` WHERE `state` = 1 AND  `is_bot` = 0;")
#define QUERY_SEL_MANAGERS                  QString("SELECT `username`, t1.`id`, `name`,  `surname`,  `patronymic`, `office`, `inform_comment`, `inform_status` FROM `users` AS t1 LEFT JOIN `roles_users` AS t2 ON t1.`id` = t2.`user_id` WHERE t1.`state` = 1 AND t2.role_id IN (3, 6) GROUP BY t1.`id`;")
#define QUERY_SEL_PRT_RQST_MANAGERS(permId) QString("SELECT t1.`username`, t1.`id` FROM `users` AS t1 LEFT JOIN `roles_users` AS t2 ON t1.id = t2.user_id LEFT JOIN `permissions_roles` AS t3 ON t2.`role_id` = t3.`role_id` WHERE t3.`permission_id` = %1 GROUP BY t1.`id`;").arg((permId))
#define QUERY_SEL_ENGINEERS                 QString("SELECT `username`, t1.`id`, `name`,  `surname`,  `patronymic`, `office`, `inform_comment`, `inform_status` FROM `users` AS t1 LEFT JOIN `roles_users` AS t2 ON t1.`id` = t2.`user_id` WHERE t1.`state` = 1 AND t2.role_id IN (2, 5) GROUP BY t1.`id`;")
#define QUERY_SEL_ITEM_BOXES(office)        QString("SELECT t1.`name`, t1.`id`, t1.`places`, t1.`color` FROM `boxes` AS t1 LEFT JOIN `stores` AS t2 ON t1.`store_id` = t2.`id` WHERE t2.`office` = %1 AND t1.`non_items` = 0 ORDER BY t1.`name`;").arg((office))
#define QUERY_SEL_REPAIR_BOXES              QString("SELECT `name`, `id`,`places`, `color` FROM `boxes` WHERE `non_items` = 1 ORDER BY `name`;")
#define QUERY_SEL_PAYMENT_SYSTEMS           QString("SELECT `name`, `system_id`, `system_data`, `id` FROM `payment_systems` WHERE `is_enable` = 1 ORDER BY `name`;")
#define QUERY_SEL_PAYMENT_TYPE(id)          QString("SELECT `name`, `id`,  `type`,  `client`,  `periodic`,  `pay_date`,  `def_summ`,  `reason`,  `is_archive`,  `updated_at`,  `payment_system` FROM `payment_types` WHERE `id` = %1;").arg((id))
#define QUERY_SEL_EXTRA_PAYMENT_TYPES(type, archive) QString("SELECT  `name`, `id`, `type`, `client`, `periodic`, `pay_date`, `def_summ`, `reason`, `is_archive`, `updated_at`, `payment_system` FROM `payment_types` WHERE `type` = %1 AND IF(%2, 1, `is_archive` = 0);").arg((type)).arg((archive))
#define QUERY_SEL_DEVICE(id)                QString("SELECT  `id`,  `type`,  `maker`,  `model`, `serial_number`,  `client` FROM `workshop` WHERE `id` = %1").arg((id))
#define QUERY_SEL_DEVICES                   QString("SELECT `name`, `id`, `company_list` FROM `devices` WHERE `enable` = 1 AND `refill` = 0 ORDER BY `position`;")
#define QUERY_SEL_DEVICE_MAKERS(device)     QString("SELECT `name`, `id` FROM `device_makers` WHERE `id` IN (%1) ORDER BY FIELD(`id`, %1);").arg((device))
#define QUERY_SEL_DEVICE_MODELS             QString("SELECT `name`, `id` FROM `device_models` WHERE `device` = %1 AND `maker` = %2 ORDER BY `name` ASC;")
#define QUERY_SEL_DEVICE_FAULTS(device)     QString(\
                                                "SELECT '' AS 'name'\n"\
                                                "UNION ALL\n"\
                                                "(SELECT\n"\
                                                "  TRIM(SUBSTRING_INDEX(SUBSTRING_INDEX(t1.`fault_list`,',',enumerator.`number`),',',1)) AS 'name'\n"\
                                                "FROM (SELECT `fault_list` FROM `devices` WHERE `id` = %1) AS t1\n"\
                                                "JOIN enumerator\n"\
                                                "ON\n"\
                                                "  (LENGTH(REPLACE(t1.`fault_list`, ',', ''))-LENGTH(t1.`fault_list`) <= enumerator.`number`+1));")\
                                                .arg((device))

#define QUERY_SEL_DEVICE_SET(device)        QString(\
                                                "SELECT"\
                                                "  '' AS 'name'\n"\
                                                "UNION ALL\n"\
                                                "(SELECT\n"\
                                                "  TRIM(SUBSTRING_INDEX(SUBSTRING_INDEX(t1.`complect_list`,',',enumerator.`number`),',',1)) AS 'name'\n"\
                                                "FROM (SELECT `complect_list` FROM `devices` WHERE `id` = %1) AS t1\n"\
                                                "JOIN enumerator\n"\
                                                "ON\n"\
                                                "  (LENGTH(REPLACE(t1.`complect_list`, ',', ''))-LENGTH(t1.`complect_list`) <= enumerator.`number`+1));")\
                                                .arg((device))
                                                
#define QUERY_SEL_DEVICE_EXTERIOR(device)   QString(\
                                                "SELECT '' AS 'name'\n"\
                                                "UNION ALL\n"\
                                                "(SELECT\n"\
                                                "  TRIM(SUBSTRING_INDEX(SUBSTRING_INDEX(t1.`look_list`,',',enumerator.`number`),',',1)) AS 'name'\n"\
                                                "FROM (SELECT `look_list` FROM `devices` WHERE `id` = %1) AS t1\n"\
                                                "JOIN enumerator\n"\
                                                "ON\n"\
                                                "  (LENGTH(REPLACE(t1.`look_list`, ',', ''))-LENGTH(t1.`look_list`) <= enumerator.`number`+1));\n")\
                                                .arg((device))
#define QUERY_SEL_CARTRIDGE_CAT_ID              QString("SELECT `id`, `company_list` FROM `devices` WHERE `enable` = 1 AND `refill` = 1 LIMIT 1;")
#define QUERY_SEL_CARTRIDGE_MODELS(vendor)          QString("SELECT  `name`, `id`  FROM `cartridge_cards` WHERE `maker` = %1 AND `archive` = 0 ORDER BY `name` ASC;").arg((vendor))
#define QUERY_SEL_CARTRIDGE_BY_SN(where_clause)     QString("SELECT  t1.`id`, `Title`, `client`, `type`, `maker`, `cartridge`, t2.`card_id`, `serial_number`, `sms_inform`, `termsControl` FROM `workshop` AS t1 LEFT JOIN `c_workshop` AS t2 ON t1.`cartridge` = t2.`id` WHERE %1 GROUP BY `serial_number`;").arg(where_clause)
#define QUERY_SEL_CARTRIDGE_REPAIR_DATA(id)     QString("SELECT `refill`, `chip`, `opc_drum`, `c_blade`, `card_id` FROM `c_workshop` WHERE `id` = %1;").arg((id))
#define QUERY_SEL_CARTRIDGE_MATERIAL(articul, count)  QString("SELECT `id` FROM `store_items` WHERE `articul` = %1 AND `count` - `reserved` >= %2 ORDER BY `id` ASC LIMIT 1;").arg(articul).arg(count)
#define QUERY_SEL_CARTRIDGE_MATERIALS           QString("SELECT `id`, `type`, `count`, `price`, `works_price`, 0, `name`, `articul`, `salary_summ` FROM `materials`")
#define QUERY_SEL_CARTRIDGE_RESOURCE(currentRepair, serial, workType)  QString("SELECT COUNT(t1.`id`) AS 'count' FROM `workshop` AS t1 LEFT JOIN `works` AS t2 ON t1.`id` = t2.`repair` WHERE  IF(%1, t1.`id` < %1, 1) AND `serial_number` = '%2' AND `cartridge` IS NOT NULL AND t2.`type` = %3").arg(currentRepair).arg(serial).arg(workType)

#define QUERY_SEL_ADDITIONAL_FIELDS_TYPES(isRepair, id) QString(\
                                                "SELECT\n"\
                                                "  `name`,\n"\
                                                "  REPLACE(`def_values`,'\r','') AS 'def_values',\n"\
                                                "  `type`,\n"\
                                                "  `id`,\n"\
                                                "  `required`,\n"\
                                                "  `printable`,\n"\
                                                "  `dev_match`,\n"\
                                                "  `input_mask`\n"\
                                                "FROM\n"\
                                                "  `fields`\n"\
                                                "WHERE\n"\
                                                "  `_f` = %1\n"\
                                                "  AND FIND_IN_SET(%2,`%3`)\n"\
                                                "  AND `archive` = 0\n"\
                                                "ORDER BY\n"\
                                                "  `id`;")\
                                                .arg((isRepair?0:1))\
                                                .arg((id))\
                                                .arg((isRepair?"devices":"categories"))
#define QUERY_SEL_DEVICE_MATCH              QString(\
                                                "SELECT\n"\
                                                "  t1.`id`,\n"\
                                                "  CONCAT_WS(' ', t2.`name`,  t3.`name`,  t4.`name`) AS 'device',\n"\
                                                "  `fault`,\n"\
                                                "  `serial_number`,\n"\
                                                "  CONCAT_WS(' ', t5.surname, t5.name, t5.patronymic) AS 'client'\n"\
                                                "FROM\n"\
                                                "  `workshop` AS t1\n"\
                                                "LEFT JOIN `devices` AS t2\n"\
                                                "  ON t1.`type` = t2.`id`\n"\
                                                "LEFT JOIN `device_makers` AS t3\n"\
                                                "  ON t1.maker = t3.`id`\n"\
                                                "LEFT JOIN `device_models` AS t4\n"\
                                                "  ON t1.model = t4.`id`\n"\
                                                "LEFT JOIN `clients` AS t5\n"\
                                                "  ON t1.`client` = t5.`id`\n"\
                                                "WHERE\n"\
                                                "  %1\n"\
                                                "ORDER BY\n"\
                                                "  `id` DESC;")
#define QUERY_SEL_DEVICE_MATCH_BY_SN        QString(\
                                                "LCASE(`serial_number`) REGEXP LCASE('%1')")
#define QUERY_SEL_DEVICE_MATCH_BY_FIELDS     QString(\
                                                "t1.`id` IN (\n"\
                                                "    SELECT\n"\
                                                "      t1.`repair_id`\n"\
                                                "    FROM\n"\
                                                "      `field_values` AS t1\n"\
                                                "    LEFT JOIN `fields` AS t2\n"\
                                                "      ON t1.field_id = t2.id\n"\
                                                "    WHERE\n"\
                                                "      %1\n"\
                                                "    )")
#define QUERY_SEL_DEVICE_MATCH_BY_FIELD     QString(\
                                                "(\n"\
                                                "        t1.field_id = %1 AND\n"\
                                                "        LCASE(t1.`value`) REGEXP LCASE('%2')\n"\
                                                "      )")

#define QUERY_SEL_CLIENTS_STATIC            QString(                                                                           \
                                                "SELECT                                                                     \n"\
                                                "  t1.`id`,                                                                 \n"\
                                                "  IF(t1.`type`,                                                            \n"\
                                                "     IF(LENGTH(TRIM(t1.`ur_name`)), t1.`ur_name`, t1.`name`),              \n"\
                                                "     CONCAT_WS(' ', t1.`surname`, t1.`name`, t1.`patronymic`)) AS 'FIO',   \n"\
                                                "  t1.`balance`,                                                            \n"\
                                                "  t1.`repairs`,                                                            \n"\
                                                "  t1.`purchases`,                                                          \n"\
                                                "  IF(t1.`type` = 1, 'Ю', '') AS 'type',                                    \n"\
                                                "  IFNULL(t2.`phone`, '') AS 'phone',                                       \n"\
                                                "  IFNULL(t1.`email`, '') AS 'email',                                       \n"\
                                                "  t1.`state`,                                                              \n"\
                                                "  t1.`short_name`                                                          \n"\
                                                "FROM `clients` AS t1                                                       \n"\
                                                "LEFT JOIN `tel` AS t2                                                      \n"\
                                                "ON t1.`id` = t2.`customer`                                                 \n"\
                                                "  AND t2.`type` = 1                                                          "\
                                            )

#define QUERY_SEL_WORKSHOP_STATIC           QString(\
                                                "SELECT\n"\
                                                "   CONCAT_WS(' ', IF(`is_warranty`, 'Г', ''), IF(`is_repeat`, 'П', ''), IF(`express_repair`, 'С', ''), IF(`informed_status`, '*', '')) AS 'marks',\n"\
                                                "   t1.`id`,\n"\
                                                "   `Title`,\n"\
                                                "   `serial_number`,\n"\
                                                "   `fault`,\n"\
                                                "   `master`,\n"\
                                                "   t1.`state` AS 'status',\n"\
                                                "   `real_repair_cost`,\n"\
                                                "   `in_date`,\n"\
                                                "   `out_date`,\n"\
                                                "   IF(t5.`type`, IF(LENGTH(TRIM(t5.`ur_name`)), t5.`ur_name`, t5.`name`), CONCAT_WS(' ', t5.`surname`, t5.`name`, t5.`patronymic`)) AS 'client',\n"\
                                                "   t6.`phone`,\n"\
                                                "   `box`,\n"\
                                                "   `current_manager`,\n"\
                                                "   '' AS 'Progress',\n"\
                                                "   `ext_notes`,\n"\
                                                "   t2.`name` AS 'dev_class_name',\n"\
                                                "   CONCAT_WS(' ', t2.`name`,  t3.`name`,  t4.`name`) AS 'device',\n"\
                                                "   `office`,\n"\
                                                "   t1.`state`,\n"\
                                                "   `new_state`,\n"\
                                                "   `user_lock`,\n"\
                                                "   `lock_datetime`,\n"\
                                                "   `quick_repair`,\n"\
                                                "   `thirs_party_sc`,\n"\
                                                "   `last_save`,\n"\
                                                "   `last_status_changed`,\n"\
                                                "   `warranty_days`,\n"\
                                                "   `color`,\n"\
                                                "   `early`,\n"\
                                                "   `ext_early`,\n"\
                                                "   `issued_msg`,\n"\
                                                "   `vendor_id`,\n"\
                                                "   `termsControl`,\n"\
                                                "   `Hidden`,\n"\
                                                "   `company`,\n"\
                                                "   `informed_status`,\n"\
                                                "   `client` AS 'client_id',\n"\
                                                "   t5.`short_name` AS 'client_short_name'\n"\
                                                "FROM `workshop` AS t1\n"\
                                                "LEFT JOIN `devices` AS t2\n"\
                                                "   ON t1.`type` = t2.`id`\n"\
                                                "LEFT JOIN `device_makers` AS t3\n"\
                                                "   ON t1.maker = t3.`id`\n"\
                                                "LEFT JOIN `device_models` AS t4\n"\
                                                "   ON t1.model = t4.`id`\n"\
                                                "LEFT JOIN `clients` AS t5\n"\
                                                "   ON t1.`client` = t5.`id`\n"\
                                                "LEFT JOIN `tel` AS t6\n"\
                                                "   ON t5.`id` = t6.`customer` AND t6.`type` = 1\n")   // Здесь не должно быть точки с запятой!

#define QUERY_SEL_REPAIR_DATA(id)           QString(\
                                                "SELECT\n"\
                                                "  t1.*,\n"\
                                                "  t2.`refill` AS 'is_cartridge'\n"\
                                                "FROM\n"\
                                                "  workshop AS t1\n"\
                                                "  LEFT JOIN `devices` AS t2\n"\
                                                "  ON t1.`type` = t2.`id`\n"\
                                                "WHERE\n"\
                                                "  t1.`id` = %1;")\
                                                .arg((id))

#define QUERY_SEL_COMMENTS(field, id)           QString("SELECT `id`, `created`, `user`, `text` FROM `comments` WHERE `%1` = %2 ORDER BY `created` DESC,`id` DESC;").arg((field)).arg((id)), QSqlDatabase::database("connMain")

#define QUERY_SEL_REPAIR_WORKS_AND_PARTS(id)    QString(\
                                                "SELECT `id`, `UID`, `name`, `count`, `avail`, `price`, `summ`, `box`, `sn`, `warranty`, `user`, `is_realization`, `return_percent`, `state`, `notes`, `item_id`, `in_price`, `obj_id`, `dealer`, `buyer`, `created`, `work_id`, `is_item`, `work_type` FROM (\n"\
                                                "SELECT\n"\
                                                "  t1.`id`,\n"\
                                                "  t1.`price_id` AS 'UID',\n"\
                                                "  t1.`name`,\n"\
                                                "  t1.`count`,\n"\
                                                "  999999 AS 'avail',\n"\
                                                "  ROUND(t1.`price`, 2) AS 'price',\n"\
                                                "  t1.`count`*ROUND(t1.`price`, 2) AS 'summ',\n"\
                                                "  NULL AS 'box',\n"\
                                                "  '' AS 'sn',\n"\
                                                "  t1.`warranty`,\n"\
                                                "  t1.`user`,\n"\
                                                "  0 AS 'is_realization',\n"\
                                                "  0 AS 'return_percent',\n"\
                                                "  2 AS 'state',\n"\
                                                "  '' AS 'notes',\n"\
                                                "  0 AS 'item_id',\n"\
                                                "  0 AS 'in_price',\n"\
                                                "  t1.`repair` AS 'obj_id',\n"\
                                                "  0 AS 'dealer',\n"\
                                                "  0 AS 'buyer',\n"\
                                                "  NULL AS 'created',\n"\
                                                "  `id` AS 'work_id',\n"\
                                                "  0 AS 'item_rsrv_id',\n"\
                                                "  0 AS 'is_item',\n"\
                                                "  t1.`type` AS 'work_type'\n"\
                                                "FROM\n"\
                                                "  `works` AS t1\n"\
                                                "WHERE\n"\
                                                "  `repair` = %1\n"\
                                                "UNION ALL\n"\
                                                "SELECT\n"\
                                                "  t2.`id`,\n"\
                                                "  CONCAT(LPAD(t3.articul, 6, '0'), '-', LPAD(t3.id, 6, '0')),\n"\
                                                "  t2.`name`,\n"\
                                                "  t2.`count`,\n"\
                                                "  t3.`count` - t3.`reserved` + t2.`count`,\n"\
                                                "  ROUND(t2.`price`, 2),\n"\
                                                "  t2.`count`*ROUND(t2.`price`, 2),\n"\
                                                "  t3.`box`,\n"\
                                                "  t2.`sn`,\n"\
                                                "  t2.`warranty`,\n"\
                                                "  t2.`to_user`,\n"\
                                                "  t3.`is_realization`,\n"\
                                                "  t3.`return_percent`,\n"\
                                                "  t2.`state`,\n"\
                                                "  t2.`notes`,\n"\
                                                "  t3.`id`,\n"\
                                                "  t3.`in_price`,\n"\
                                                "  t2.`repair_id`,\n"\
                                                "  t3.`dealer`,\n"\
                                                "  0,\n"\
                                                "  t2.`created`,\n"\
                                                "  t2.`work_id`,\n"\
                                                "  t2.`id`,\n"\
                                                "  1,\n"\
                                                "  NULL\n"\
                                                "FROM\n"\
                                                "  `store_int_reserve` AS t2\n"\
                                                "  LEFT JOIN\n"\
                                                "  `store_items` AS t3 ON t2.`item_id` = t3.`id`\n"\
                                                "WHERE\n"\
                                                "  t2.work_id IN (SELECT `id` FROM `works` WHERE `repair` = %1) OR t2.`repair_id` = %1) AS `tbl`\n"\
                                                "ORDER BY\n"\
                                                "  `work_id` ASC,\n"\
                                                "  `item_rsrv_id` ASC;")\
                                                .arg((id))

#define QUERY_SEL_REPAIR_PAYMENTS(R, T)     QString("SELECT IFNULL(SUM(`summa`), 0) AS 'summa' FROM `cash_orders` WHERE `repair` = %1 AND `type` = IF(%2, %2, `type`);").arg((R)).arg((T))
#define QUERY_SEL_REPAIR_WORKS(R)           QString("SELECT SUM(`price` * `count`) AS 'summa' FROM `works` WHERE `repair` = %1;").arg((R))
#define QUERY_SEL_REPAIR_PARTS(R)           QString("SELECT SUM(`price` * `count`) AS `summa` FROM `store_int_reserve` WHERE `state` IN (2, 3) AND `repair_id` = %1;").arg((R))
#define QUERY_SEL_REPAIR_MNGR_ENGR(R)       QString("SELECT `current_manager`, `master` FROM workshop WHERE `id` = %1").arg((R))
#define QUERY_SEL_REPAIR_LOCK(db, id)       QString("SELECT IFNULL(t1.`user_lock`, 0) AS 'user_id', IF(t3.`USER` IS NULL, 0, 1) AS 'is_user_online', t1.`lock_datetime` FROM `%1`.`workshop` AS t1 LEFT JOIN `%1`.`users` AS t2 ON t1.`user_lock` = t2.`id` LEFT JOIN `information_schema`.`PROCESSLIST` AS t3 ON t2.`username` = t3.`USER` AND  t3.`DB` = '%1' WHERE t1.`id` = %2 GROUP BY t1.`id`;").arg((db)).arg((id))
#define QUERY_SEL_REPAIR_ADD_FIELDS(R)      QString("SELECT t1.`id`, t2.`name`, t1.`value`, t1.`field_id`, t1.`repair_id`, t1.`item_id`, '' AS 'comment', t2.`printable` FROM `field_values` AS t1 LEFT JOIN `fields` AS t2 ON t1.`field_id` = t2.`id` WHERE t1.`repair_id` = %1 ORDER BY t1.`field_id` ASC;").arg((R))
#define QUERY_SEL_ADD_FIELD(id)             QString("SELECT t1.`id`, t2.`name`, t1.`value`, t1.`field_id`, t1.`repair_id`, t1.`item_id`, '' AS 'comment', t2.`printable` FROM `field_values` AS t1 LEFT JOIN `fields` AS t2 ON t1.`field_id` = t2.`id` WHERE t1.`id` = %1;").arg((id))
#define QUERY_SEL_ITEM_ADD_FIELDS(I)        QString("SELECT t1.`id`, t2.`name`, t1.`value`, t1.`field_id`, t1.`repair_id`, t1.`item_id`, '' AS 'comment', t2.`printable` FROM `field_values` AS t1 LEFT JOIN `fields` AS t2 ON t1.`field_id` = t2.`id` WHERE t1.`item_id` = %1 ORDER BY t1.`field_id` ASC;").arg((I))
#define QUERY_SEL_IS_BALANCE_EN(C)          QString("SELECT SUM(`summ`) AS 'summa' FROM `balance` WHERE `client` = %1;").arg((C))
#define QUERY_SEL_DOC(id)                   QString("SELECT  `id`,  `type`,  `state`,  `is_realization`,  `payment_system`,  `company`,  `store`,  `user`,  `total`,  `notes`,  `created`,  `updated_at`,  `office`,  `dealer`,  `currency_rate`,  `reason`,  `order_id`,  `price_option`,  `return_percent`,  `reserve_days`, DiffDays(UTC_TIMESTAMP(), `created`) AS 'diff_days', `master_id`,  `repair_id`,  `works_included`,  `invoice`,  `track`,  `d_store`,  `d_pay` FROM `docs` WHERE `id` = %1;").arg((id))

#define QUERY_SEL_ITEMS_IN_DOC(id)                      QString(\
                                                        "SELECT\n"\
                                                        "	t1.`id`,\n"\
                                                        "	CONCAT(LPAD(t2.articul, 6, '0'), '-', LPAD(t2.id, 6, '0')) AS 'UID',\n"\
                                                        "	t2.`name`,\n"\
                                                        "	t1.`count`,\n"\
                                                        "	t2.`count` - t2.`reserved` AS 'avail',\n"\
                                                        "	t1.`price`,\n"\
                                                        "	t1.`count`*t1.`price` AS 'summ',\n"\
                                                        "	t2.`box`,\n"\
                                                        "	t1.`sn`,\n"\
                                                        "	t1.`warranty`,\n"\
                                                        "	t1.`user`,\n"\
                                                        "	t2.`is_realization`,\n"\
                                                        "	t2.`return_percent`,\n"\
                                                        "	t1.`is_cancellation`,\n"\
                                                        "	t1.`cancellation_reason`,\n"\
                                                        "	t2.`id` AS 'item_id',\n"\
                                                        "	t2.`in_price`,\n"\
                                                        "	t1.`document_id` AS 'obj_id',\n"\
                                                        "	t2.`dealer`,\n"\
                                                        "	t1.`customer_id` AS 'buyer',\n"\
                                                        "	NULL AS 'created',\n"\
                                                        "	NULL AS 'work_id',\n"\
                                                        "	1 AS 'is_item',\n"\
                                                        "	NULL AS 'work_type'\n"\
                                                        "FROM\n"\
                                                        "	store_sales AS t1 LEFT JOIN\n"\
                                                        "	store_items AS t2\n"\
                                                        "	ON\n"\
                                                        "	t1.`item_id`=t2.id\n"\
                                                        "WHERE\n"\
                                                        "(`document_id` = %1);").arg((id))
                                                        
#define QUERY_SEL_ITEMS_IN_DOC_RSRV(id)                 QString(\
                                                        "SELECT\n"\
                                                        "  t1.`id`,\n"\
                                                        "  CONCAT(LPAD(t2.articul, 6, '0'), '-', LPAD(t2.id, 6, '0')) AS 'UID',\n"\
                                                        "  t2.`name`,\n"\
                                                        "  t1.`count`,\n"\
                                                        "  t2.`count` - t2.`reserved` + t1.`count` AS 'avail',\n"\
                                                        "  t1.`price`,\n"\
                                                        "  t1.`count`*t1.`price` AS 'summ',\n"\
                                                        "  t2.`box`,\n"\
                                                        "  t1.`sn`,\n"\
                                                        "  t1.`warranty`,\n"\
                                                        "  t1.`user`,\n"\
                                                        "  t2.`is_realization`,\n"\
                                                        "  t2.`return_percent`,\n"\
                                                        "  t1.`is_cancellation` AS 'state',\n"\
                                                        "  t1.`cancellation_reason` AS 'notes',\n"\
                                                        "  t2.`id` AS 'item_id',\n"\
                                                        "  t2.`in_price`,\n"\
                                                        "  t1.`document_id` AS 'obj_id',\n"\
                                                        "  t2.`dealer`,\n"\
                                                        "  t1.`customer_id` AS 'buyer',\n"\
                                                        "  NULL AS 'created',\n"\
                                                        "  NULL AS 'work_id',\n"\
                                                        "  1 AS 'is_item',\n"\
                                                        "  NULL AS 'work_type'\n"\
                                                        "FROM\n"\
                                                        "  store_sales AS t1 LEFT JOIN\n"\
                                                        "  store_items AS t2\n"\
                                                        "  ON\n"\
                                                        "    t1.`item_id`=t2.id\n"\
                                                        "WHERE\n"\
                                                        "  (`document_id` = %1);")\
                                                        .arg((id))
                                                        
#define QUERY_SEL_PART_WAREHOUSE(uid)                   QString(\
                                                        "SELECT\n"\
                                                        "  t2.`office`\n"\
                                                        "FROM\n"\
                                                        "  store_items AS t1\n"\
                                                        "  LEFT JOIN stores AS t2\n"\
                                                        "    ON t1.`store` = t2.`id`\n"\
                                                        "WHERE\n"\
                                                        "  t1.`id` IN (%1);")\
                                                        .arg((uid))

#define QUERY_SEL_PART_FOR_SALE(uid, price_field_name, count)  QString(\
                                                        "SELECT\n"\
                                                        "  0 AS 'id',\n"\
                                                        "  CONCAT(LPAD(articul, 6, '0'), '-', LPAD(id, 6, '0')) AS 'UID',\n"\
                                                        "  `name`,\n"\
                                                        "  %3 AS 'count',\n"\
                                                        "  `count` - `reserved` AS 'avail',\n"\
                                                        "  %2 AS 'price',\n"\
                                                        "  %2*%3 AS 'summ',\n"\
                                                        "  `box`,\n"\
                                                        "  `SN` AS 'sn',\n"\
                                                        "  `warranty`,\n"\
                                                        "  0 AS 'user',\n"\
                                                        "  `is_realization`,\n"\
                                                        "  `return_percent`,\n"\
                                                        "  0 AS 'state',\n"\
                                                        "  NULL  AS 'notes',\n"\
                                                        "  `id` AS 'item_id',\n"\
                                                        "  `in_price`,\n"\
                                                        "  0 AS 'obj_id',\n"\
                                                        "  `dealer`,\n"\
                                                        "  0 AS 'buyer',\n"\
                                                        "  NULL AS 'created',\n"\
                                                        "  NULL AS 'work_id',\n"\
                                                        "  1 AS 'is_item',\n"\
                                                        "  NULL AS 'work_type'\n"\
                                                        "FROM\n"\
                                                        "  store_items\n"\
                                                        "WHERE\n"\
                                                        "  `id` IN (%1);")\
                                                        .arg((uid))\
                                                        .arg((price_field_name))\
                                                        .arg((count))
#define QUERY_SEL_PART_FROM_BASKET(uid, count)  QString(\
                                                        "SELECT\n"\
                                                        "  `id`,\n"\
                                                        "  CONCAT(LPAD(`articul`, 6, '0'), '-', LPAD(`item_id`, 6, '0')) AS 'UID',\n"\
                                                        "  `name`,\n"\
                                                        "  %2 AS 'count',\n"\
                                                        "  `avail`,\n"\
                                                        "  `price`,\n"\
                                                        "  `price`*%2 AS 'summ',\n"\
                                                        "  `box`,\n"\
                                                        "  `sn`,\n"\
                                                        "  `warranty`,\n"\
                                                        "  `to_user` AS 'user',\n"\
                                                        "  `is_realization`,\n"\
                                                        "  `return_percent`,\n"\
                                                        "  0 AS 'state',\n"\
                                                        "  `notes` AS 'notes',\n"\
                                                        "  `item_id`,\n"\
                                                        "  `in_price`,\n"\
                                                        "  `repair_id` AS 'obj_id',\n"\
                                                        "  `dealer`,\n"\
                                                        "  0 AS 'buyer',\n"\
                                                        "  `created`,\n"\
                                                        "  NULL AS 'work_id',\n"\
                                                        "  1 AS 'is_item',\n"\
                                                        "  NULL AS 'work_type'\n"\
                                                        "FROM (\n"\
                                                        "  SELECT t1.`id`,  t1.`item_id`,  t1.`name`,  t1.`count`,  t1.`created`,  t1.`from_user`,  t1.`to_user`,  t1.`notes`,  t1.`state`,  t1.`repair_id`,  t1.`work_id`,  t1.`sn`,  t1.`warranty`, t2.`articul`, t2.`count` - t2.`reserved` + t1.`count` AS 'avail', t1.`price`, t2.`in_price`, t2.`document`, t2.`box`, t2.`is_realization`, t2.`return_percent`, t3.`dealer` FROM\n"\
                                                        "  store_int_reserve AS t1 \n"\
                                                        "  LEFT JOIN store_items AS t2\n"\
                                                        "    ON t1.`item_id` = t2.`id`\n"\
                                                        "  LEFT JOIN docs AS t3\n"\
                                                        "    ON t2.`document` = t3.`id`\n"\
                                                        "  ) AS `item`\n"\
                                                        "WHERE\n"\
                                                        "  `id` IN (%1);")\
                                                        .arg((uid))\
                                                        .arg((count))

#define QUERY_SEL_ITEM_ACTUAL_QTY(item_id)   QString(\
                                                        "SELECT\n"\
                                                        "  `count`,\n"\
                                                        "  `sold`,\n"\
                                                        "  `reserved`\n"\
                                                        "FROM\n"\
                                                        "  `store_items`\n"\
                                                        "WHERE\n"\
                                                        "  `id` = %1;")\
                                                        .arg((item_id))

#define QUERY_SEL_STORE_SALES_QTY(id)       QString("SELECT `count` FROM store_sales WHERE `id` = %1;").arg((id))
#define QUERY_SEL_REPAIR_SALES_QTY(id)       QString("SELECT `count` FROM store_int_reserve WHERE `id` = %1;").arg((id))
#define QUERY_SEL_CASH_ORDER(id)            QString(\
                                                        "SELECT\n"\
                                                        "  `id`,\n"\
                                                        "  `created`,\n"\
                                                        "  `type`,\n"\
                                                        "  `summa`,\n"\
                                                        "  `summa_str`,\n"\
                                                        "  `invoice`,\n"\
                                                        "  `client`,\n"\
                                                        "  `to_user`,\n"\
                                                        "  `user`,\n"\
                                                        "  `company`,\n"\
                                                        "  `office`,\n"\
                                                        "  `notes`,\n"\
                                                        "  `repair`,\n"\
                                                        "  `document`,\n"\
                                                        "  `img`,\n"\
                                                        "  `payment_system`,\n"\
                                                        "  `card_fee`,\n"\
                                                        "  `is_backdate`,\n"\
                                                        "  `card_info`,\n"\
                                                        "  `customer_email`,\n"\
                                                        "  `fdn`,\n"\
                                                        "  `payment_item_sign`\n"\
                                                        "FROM\n"\
                                                        "  `cash_orders`\n"\
                                                        "WHERE\n"\
                                                        "  `id` = %1;").arg((id))
#define QUERY_SEL_INVOICE(id)               QString(\
                                                        "SELECT\r\n"\
                                                        "  t1.`id`,\r\n"\
                                                        "  t1.`num`,\r\n"\
                                                        "  t1.`created`,\r\n"\
                                                        "  t1.`user`,\r\n"\
                                                        "  t1.`seller`,\r\n"\
                                                        "  t1.`customer` AS 'covenantor',\r\n"\
                                                        "  t2.`ur_name` AS 'covenantor_name',\r\n"\
                                                        "  t2.`client` AS 'client_id',\r\n"\
                                                        "  t1.`paid`,\r\n"\
                                                        "  t1.`tax`,\r\n"\
                                                        "  t1.`summ`,\r\n"\
                                                        "  t1.`notes`,\r\n"\
                                                        "  t1.`total`,\r\n"\
                                                        "  t1.`state`,\r\n"\
                                                        "  t1.`office`,\r\n"\
                                                        "  t1.`type`\r\n"\
                                                        "FROM\r\n"\
                                                        "  `invoice` AS t1\r\n"\
                                                        "LEFT JOIN\r\n"\
                                                        "  `banks` AS t2\r\n"\
                                                        "ON\r\n"\
                                                        "  t1.`customer` = t2.`id`\r\n"\
                                                        "WHERE\r\n"\
                                                        "  t1.`id` = %1;")\
                                                        .arg((id))

/**************** Queries for data insertion ******************/
#define QUERY_SEL_LAST_INSERT_ID            QString("SELECT LAST_INSERT_ID();")

// ACHTUNG! ACHTUNG! если в выражениях ниже не закомментирован вызов функции qDebug(), а выражение используется в теле условного оператора БЕЗ ЗАКЛЮЧЕНИЯ в {}, то работа программы будет неправильной
#define QUERY_EXEC(obj,flag)                /*qDebug() << "(!!!) nDBErr=" << (flag) << ". Executing query...";*/ if ((flag)) (flag) = (obj)->exec

// верификация записанных в БД данных; производится примитивно, путём сравнения записанных данных с данными, которые были переданы в запросе;
// если данные совпадают, то сервер должен вернуть 55AAh (21930d), иначе 00h
// далее, если контрольное число совпадает, flag устанавливаем в 1, иначе — 0.
#define QUERY_EXEC_VRFY(obj,flag)           if ((flag)){\
                                                (obj)->first();\
                                                /*qDebug() << "Integrity magic number: " << (obj)->value(0).toInt();*/\
                                                (flag) = ((obj)->value(0).toInt() == 21930)?1:0;}

#define QUERY_NEW_ID(col,tbl,obj,flag,id)   if ((flag)){\
                                                (flag) = (obj)->exec("SELECT (MAX(`" + (col) + "`)) + 1 AS 'id', COUNT(1) FROM `" + (tbl) + "`;");\
                                                (obj)->first();\
                                                (id) = (obj)->value(0).toInt();}

// QSqlQuery::lastInsertId() почему-то не работает, поэтому так:
#define QUERY_LAST_INS_ID(obj,flag,id)      /*qDebug() << "(!!!) nDBErr=" << (flag) << ". Executing SELECT LAST_INSERT_ID() query...";*/ if ((flag)){\
                                                (obj)->exec("SELECT LAST_INSERT_ID();");\
                                                (obj)->first();\
                                                (id) = (obj)->value(0).toInt();}

#define QUERY_BEGIN                         QString("BEGIN;")
#define QUERY_COMMIT                        QString("COMMIT;")
#define QUERY_ROLLBACK                      QString("ROLLBACK;")
#define QUERY_ROLLBACK_MSG(obj,text)        qDebug() << QString("Возникла ошибка: %1").arg((text));\
                                            QMessageBox msgBox;\
                                            msgBox.setText(QString("Возникла ошибка\r\n%1").arg((text)));\
                                            obj->exec(QUERY_ROLLBACK);\
                                            msgBox.exec();
#define QUERY_COMMIT_ROLLBACK(obj,flag)     if (flag == 0)   /* в случае ошибки выполнения запроса нужно всё откатить */\
                                            {\
                                                obj->exec(QUERY_ROLLBACK);\
                                            }\
                                            else\
                                            {\
                                                obj->exec(QUERY_COMMIT);\
                                            }
#define QUERY_COMMIT_ROLLBACK_MSG(obj,flag) if (flag == 0)   /* в случае ошибки выполнения запроса нужно всё откатить */\
                                            {\
                                                qDebug() << QString("Ошибка выполнения запроса: %1").arg(obj->lastError().text());\
                                                QMessageBox msgBox;\
                                                msgBox.setText(QString("Ошибка выполнения запроса\r\n%1").arg(obj->lastError().text()));\
                                                obj->exec(QUERY_ROLLBACK);\
                                                msgBox.exec();\
                                            }\
                                            else\
                                            {\
                                                obj->exec(QUERY_COMMIT);\
                                            }


#define QUERY_VRFY_CASH(T,S,s,C,U,O,t,R,D,A)  QString(\
                                                "SELECT IF(`type` = %1 AND `summa` = %2 /*AND `summa_str` = '%3'*/ AND IFNULL(`client`,'NULL') = %4 AND `user` = %5 AND `office` = %6 /*AND `notes` = '%7'*/ AND IFNULL(`repair`,'NULL') = IFNULL(%8,'NULL') AND IFNULL(`document`,'NULL') = IFNULL(%9,'NULL') AND `payment_system` = %10, 21930, 0)\n"\
                                                "FROM `cash_orders`\n"\
                                                "WHERE `id` = LAST_INSERT_ID();")\
                                                .arg(T)\
                                                .arg(S)\
                                                .arg(s)\
                                                .arg(C)\
                                                .arg(U)\
                                                .arg(O)\
                                                .arg(t)\
                                                .arg(R)\
                                                .arg(D)\
                                                .arg(A)

#define QUERY_UPD_REPAIR_STATE(R, S)        QString("UPDATE `workshop` SET `out_date`=UTC_TIMESTAMP(), `state`=%1, `lock_datetime`=UTC_TIMESTAMP(), `box`=NULL, `last_status_changed`=UTC_TIMESTAMP() WHERE `id` = %2;").arg(S)\
                                                .arg(R)

#define QUERY_SET_REJ_REASON(R, t)          QString("UPDATE `workshop` SET `reject_reason` = '%2' WHERE `id` = %1;").arg((R)).arg((t))

#define QUERY_UPDATE_BALANCE(C,S)           QString(\
                                                "UPDATE\n"\
                                                "  `clients`\n"\
                                                "SET\n"\
                                                "  `balance` = %2\n"\
                                                "WHERE\n"\
                                                "  `id`=%1;")\
                                                .arg((C))\
                                                .arg((S), 0, 'f', 4)

#define QUERY_VRFY_BALANCE(C)               QString(\
                                                "SELECT\n"\
                                                "  IF(ROUND(t1.`balance`, 2) = ROUND(SUM(t2.`summ`), 2), 21930, 0) AS 'control',\n"\
                                                "  t1.`balance`\n"\
                                                "FROM\n"\
                                                "  `clients` AS t1\n"\
                                                "LEFT JOIN\n"\
                                                "  `balance` AS t2\n"\
                                                "ON\n"\
                                                "  t1.id = t2.`client`\n"\
                                                "WHERE\n"\
                                                "  t1.`balance_enable` = 1\n"\
                                                "  AND t1.`id` = %1;")\
                                                .arg((C))

// для интерактивного обновления цен уже добавленных в список товаров используется prepaired statement
#define QUERY_SEL_STORE_ITEMS_ITEM_PRICE(column)    QString("SELECT %1 FROM store_items WHERE `id` = :id;").arg(column)

#define QUERY_SEL_SALARY_REPAIRS(date1, date2, statuses, user)    QString(                                                                                                                                                \
                                                "SELECT                                                                                                                                                                \n"\
                                                "    `id`,                                                                                                                                                             \n"\
                                                "    `Title`,                                                                                                                                                          \n"\
                                                "    `real_repair_cost`,                                                                                                                                               \n"\
                                                "    SUM(`work_by_employee`) AS 'works_by_employee',                                                                                                                   \n"\
                                                "    `parts_cost`,                                                                                                                                                     \n"\
                                                "    SUM(`employee_parts`) AS 'employee_parts',                                                                                                                        \n"\
                                                "    SUM(`employee_salary_for_work`) AS 'employee_salary_for_works',                                                                                                   \n"\
                                                "    SUM(`employee_salary_for_parts`) AS 'employee_salary_for_parts',                                                                                                  \n"\
                                                "    `out_date`,                                                                                                                                                       \n"\
                                                "    `state`,                                                                                                                                                          \n"\
                                                "    `summ`,                                                                                                                                                           \n"\
                                                "    `status_id`,                                                                                                                                                      \n"\
                                                "    `filter1`,                                                                                                                                                        \n"\
                                                "    `cartridge`                                                                                                                                                       \n"\
                                                "FROM (                                                                                                                                                                \n"\
                                                "    SELECT                                                                                                                                                            \n"\
                                                "        g1t1.*,                                                                                                                                                       \n"\
                                                "        g1t2.`created_at`,                                                                                                                                            \n"\
                                                "        (g1t3.`price`*g1t3.`count`) AS 'work_by_employee',                                                                                                            \n"\
                                                "        CAST((IF((g1t1.`cartridge` AND g1t3.`salary_summ` IS NOT NULL), g1t3.`salary_summ`, (g1t3.`price`*(IF(g1t1.`cartridge`, g1t5.`pay_cartridge_refill`, IF(g1t1.quick_repair, IFNULL(g1t3.`pay_repair_quick`, g1t5.`pay_repair_quick`), IFNULL(g1t3.`pay_repair`, g1t5.`pay_repair`))))/100))*g1t3.`count`) AS DECIMAL(19,4)) AS 'employee_salary_for_work', \n"\
                                                "        IFNULL(SUM(g1t4.`price`*g1t4.`count`), 0) AS 'employee_parts',                                                                                                \n"\
                                                "        CAST(IF(g1t5.`pay_4_sale_in_repair` AND g1t1.`cartridge` IS NULL, IFNULL(SUM((g1t4.`price` - g1t6.`in_price`)*g1t4.`count`*g1t5.`pay_sale`/100), 0), 0) AS DECIMAL(19,4)) AS 'employee_salary_for_parts', \n"\
                                                "        g1t7.`summ`,                                                                                                                                                  \n"\
                                                "        g1t2.`status_id`,                                                                                                                                             \n"\
                                                "        g1t2.`filter1`                                                                                                                                                \n"\
                                                "    FROM workshop AS g1t1                                                                                                                                             \n"\
                                                "    LEFT JOIN (                                                                                                                                                       \n"\
                                                "        SELECT                                                                                                                                                        \n"\
                                                "            `repair_id`                                                                                                                                               \n"\
                                                "            , `created_at`                                                                                                                                            \n"\
                                                "            , `status_id`                                                                                                                                             \n"\
                                                "            , IF(@id<>`repair_id`, @cnt:=1, @cnt:=@cnt+1) AS `filter1`                                                                                                \n"\
                                                "            , @id:=`repair_id`                                                                                                                                        \n"\
                                                "        FROM repair_status_logs CROSS JOIN (SELECT @cnt:=1, @id:=0) AS dummy                                                                                          \n"\
                                                "        ORDER BY `repair_id` ASC, `id` DESC                                                                                                                           \n"\
                                                "    ) AS g1t2                                                                                                                                                         \n"\
                                                "        ON g1t1.`id` = g1t2.`repair_id`                                                                                                                               \n"\
                                                "    LEFT JOIN works AS g1t3                                                                                                                                           \n"\
                                                "        ON g1t1.`id` = g1t3.`repair`                                                                                                                                  \n"\
                                                "    LEFT JOIN store_int_reserve AS g1t4                                                                                                                               \n"\
                                                "        ON g1t3.`id` = g1t4.`work_id` AND g1t4.`to_user` = %4                                                                                                         \n"\
                                                "    LEFT JOIN users AS g1t5                                                                                                                                           \n"\
                                                "        ON g1t3.`user` = g1t5.`id`                                                                                                                                    \n"\
                                                "    LEFT JOIN store_items AS g1t6                                                                                                                                     \n"\
                                                "        ON g1t4.`item_id` = g1t6.`id`                                                                                                                                 \n"\
                                                "    LEFT JOIN salary_repairs AS g1t7                                                                                                                                  \n"\
                                                "        ON g1t1.`id` = g1t7.`repair` AND g1t7.`user` = %4                                                                                                             \n"\
                                                "    WHERE                                                                                                                                                             \n"\
                                                "        ( ( g1t7.`id` IS NULL                                                                                                                                         \n"\
                                                "            AND                                                                                                                                                       \n"\
                                                "            (  g1t2.`filter1`  = 1                                                                                                                                    \n"\
                                                "               AND g1t2.`status_id` IN (%3)                                                                                                                           \n"\
                                                "               AND g1t2.`created_at` > '%1'                                                                                                                           \n"\
                                                "               AND g1t2.`created_at` < '%2' )                                                                                                                         \n"\
                                                "             OR                                                                                                                                                       \n"\
                                                "             ( g1t2.`filter1` IS NULL                                                                                                                                 \n"\
                                                "               AND g1t1.`out_date` > '%1'                                                                                                                             \n"\
                                                "               AND g1t1.`out_date` < '%2' )                                                                                                                           \n"\
                                                "          )                                                                                                                                                           \n"\
                                                "          OR                                                                                                                                                          \n"\
                                                "          ( g1t7.`id` IS NOT NULL                                                                                                                                     \n"\
                                                "            AND (g1t2.`filter1`  = 1 OR g1t2.`filter1` IS NULL)                                                                                                       \n"\
                                                "            AND g1t7.`accounting_date` > '%1'                                                                                                                         \n"\
                                                "            AND g1t7.`accounting_date` < '%2' ))                                                                                                                      \n"\
                                                "        AND g1t3.`user` = %4                                                                                                                                          \n"\
                                                "    GROUP BY g1t3.id                                                                                                                                                  \n"\
                                                ") AS t1                                                                                                                                                               \n"\
                                                )\
                                                .arg(date1)\
                                                .arg(date2)\
                                                .arg(statuses)\
                                                .arg(user),\
                                                QSqlDatabase::database("connMain")

#define QUERY_SEL_SALARY_REPAIR_WORKS(id, user)    QString(                                                                                      \
                                                "SELECT                                                                                       \n"\
                                                "    t1.`price_id`,                                                                           \n"\
                                                "    t1.`name`,                                                                               \n"\
                                                "    t1.`count`,                                                                              \n"\
                                                "    t1.`price`,                                                                              \n"\
                                                "    t1.`count`*t1.`price` AS 'summ',                                                         \n"\
                                                "    t1.`warranty`,                                                                           \n"\
                                                "    IF((t2.`cartridge` AND t1.`salary_summ` IS NOT NULL), '-', (@percent := IF(t2.`cartridge`, t3.`pay_cartridge_refill`, IF(t2.quick_repair, IFNULL(t1.`pay_repair_quick`, t3.`pay_repair_quick`), IFNULL(t1.`pay_repair`, t3.`pay_repair`))))) AS 'percent',    \n"\
                                                "    CAST(IF((t2.`cartridge` AND t1.`salary_summ` IS NOT NULL), t1.`salary_summ`, (t1.`price`*@percent/100))*t1.`count` AS DECIMAL(19,4)) AS 'salary_part'                                      \n"\
                                                "FROM works AS t1                                                                             \n"\
                                                "LEFT JOIN workshop AS t2                                                                     \n"\
                                                "    ON t1.`repair` = t2.`id`                                                                 \n"\
                                                "LEFT JOIN users AS t3                                                                        \n"\
                                                "    ON t1.`user` = t3.`id`                                                                   \n"\
                                                "WHERE                                                                                        \n"\
                                                "    t1.`repair` = %1                                                                         \n"\
                                                "    AND t1.`user` = %2                                                                       \n"\
                                                )\
                                                .arg(id)\
                                                .arg(user),\
                                                QSqlDatabase::database("connMain")

#define QUERY_SEL_SALARY_REPAIR_PARTS(id, user)    QString(                                                                                      \
                                                "SELECT                                                                                       \n"\
                                                "    CONCAT(t2.`articul`, '-', t1.`item_id`) AS 'UID',                                        \n"\
                                                "    t1.`name`,                                                                               \n"\
                                                "    t1.`count`,                                                                              \n"\
                                                "    t1.`price`,                                                                              \n"\
                                                "    @summ:=t1.`count` * t1.`price` AS 'summ',                                                \n"\
                                                "    t1.`sn`,                                                                                 \n"\
                                                "    t1.`warranty`,                                                                           \n"\
                                                "    @profit:=t1.`count`*(t1.`price`-t2.`in_price`) AS 'profit',                              \n"\
                                                "    IF(t4.`cartridge`, '-', CAST(@profit*IF(t3.`pay_4_sale_in_repair`, t3.`pay_sale`, 0)/100 AS DECIMAL(19,4))) AS 'salary_part', \n"\
                                                "    t1.`item_id`,                                                                            \n"\
                                                "    t2.`is_realization`                                                                      \n"\
                                                "FROM store_int_reserve AS t1                                                                 \n"\
                                                "LEFT JOIN store_items AS t2                                                                  \n"\
                                                "    ON t1.`item_id` = t2.`id`                                                                \n"\
                                                "LEFT JOIN `users` AS t3                                                                      \n"\
                                                "     ON t1.`to_user` = t3.`id`                                                               \n"\
                                                "LEFT JOIN `workshop` AS t4                                                                   \n"\
                                                "     ON t1.`repair_id` = t4.`id`                                                             \n"\
                                                "WHERE                                                                                        \n"\
                                                "    `repair_id` = %1                                                                         \n"\
                                                "    AND `to_user` = %2                                                                       \n"\
                                                )\
                                                .arg(id)\
                                                .arg(user),\
                                                QSqlDatabase::database("connMain")

#define QUERY_SEL_SALARY_EXTRA_CHARGES(date1, date2, user)    QString(                                             \
                                                "SELECT                                                         \n"\
                                                "  `id`,                                                        \n"\
                                                "  `name`,                                                      \n"\
                                                "  `price`,                                                     \n"\
                                                "  `payment_date`,                                              \n"\
                                                "  `user`                                                       \n"\
                                                "FROM `additional_payments`                                     \n"\
                                                "WHERE                                                          \n"\
                                                "  `payment_date` >= '%1'                                       \n"\
                                                "  AND `payment_date` <= '%2'                                   \n"\
                                                "  AND `to_user` = %3                                             "\
                                                )\
                                                .arg(date1)\
                                                .arg(date2)\
                                                .arg(user),\
                                                QSqlDatabase::database("connMain")

#define QUERY_SEL_SALARY_SALE_DOCS(date1, date2, user)    QString(                                                                   \
                                                "SELECT                                                                           \n"\
                                                "  `salary`.`id`,                                                                 \n"\
                                                "  `salary`.`created`,                                                            \n"\
                                                "  `salary`.`client`,                                                             \n"\
                                                "  `salary`.`total`,                                                              \n"\
                                                "  `salary`.`profit`,                                                             \n"\
                                                "  `salary`.`profit`*`users`.`pay_sale`/100 AS 'salary_part'                      \n"\
                                                "FROM (                                                                           \n"\
                                                "  SELECT                                                                         \n"\
                                                "    t1.`id`,                                                                     \n"\
                                                "    t1.`created`,                                                                \n"\
                                                "    CONCAT_WS(' ', t2.surname, t2.`name`, t2.patronymic) AS 'client',            \n"\
                                                "    t1.`total`,                                                                  \n"\
                                                "    SUM(t3.`count`*(t3.`price`-t3.`in_price`)) AS 'profit',                      \n"\
                                                "    t1.`user`                                                                    \n"\
                                                "  FROM `docs` AS `t1`                                                            \n"\
                                                "  LEFT JOIN `clients` AS t2                                                      \n"\
                                                "    ON t1.`dealer` = t2.`id`                                                     \n"\
                                                "  LEFT JOIN `store_sales` AS t3                                                  \n"\
                                                "    ON t1.id = t3.`document_id`                                                  \n"\
                                                "  LEFT JOIN `store_items` AS t4                                                  \n"\
                                                "    ON t3.`item_id` = t4.`id`                                                    \n"\
                                                "  WHERE                                                                          \n"\
                                                "    t1.`type` = 2                                                                \n"\
                                                "    AND t1.`state` = 5                                                           \n"\
                                                "    AND t1.`created` >= '%1'                                                     \n"\
                                                "    AND t1.`created` <= '%2'                                                     \n"\
                                                "    AND t1.`user` = %3                                                           \n"\
                                                "  GROUP BY t1.`id`                                                               \n"\
                                                ") AS `salary`                                                                    \n"\
                                                "LEFT JOIN `users`                                                                \n"\
                                                "  ON `salary`.`user` = `users`.`id`                                              "\
                                                )\
                                                .arg(parentTab->periodBegin())\
                                                .arg(parentTab->periodEnd())\
                                                .arg(parentTab->employeeId()),\
                                                QSqlDatabase::database("connMain")

#define QUERY_SEL_SALARY_SALE_PARTS(doc)    QString(                                                                                 \
                                                "SELECT                                                                           \n"\
                                                "  CONCAT(t2.`articul`, '-', t1.`item_id`) AS 'UID',                              \n"\
                                                "  t2.`name`,                                                                     \n"\
                                                "  t1.`count`,                                                                    \n"\
                                                "  t1.`price`,                                                                    \n"\
                                                "  @summ:=t1.`count` * t1.`price` AS 'summ',                                      \n"\
                                                "  t1.`sn`,                                                                       \n"\
                                                "  t1.`warranty`,                                                                 \n"\
                                                "  @profit:=t1.`count`*(t1.`price`-t2.`in_price`) AS 'profit',                    \n"\
                                                "  CAST(@profit*t3.`pay_sale`/100 AS DECIMAL(11,4)) AS 'salary_part',             \n"\
                                                "  t1.`item_id`                                                                   \n"\
                                                "FROM `store_sales` AS t1                                                         \n"\
                                                "LEFT JOIN `store_items` AS t2                                                    \n"\
                                                "  ON t1.`item_id` = t2.`id`                                                      \n"\
                                                "LEFT JOIN `users` AS t3                                                          \n"\
                                                "  ON t1.`user` = t3.`id`                                                         \n"\
                                                "WHERE                                                                            \n"\
                                                "  t1.`document_id` = %1"                                                            \
                                                )\
                                                .arg(doc),\
                                                QSqlDatabase::database("connMain")

#define QUERY_SEL_SALARY_PARTS(user)    QString(                                                                                 \
                                                "SELECT                                                                          \n"\
                                                "    CONCAT(t2.`articul`, '-', t1.`item_id`) AS 'UID',                           \n"\
                                                "    t1.`name`,                                                                  \n"\
                                                "    t1.`created`,                                                               \n"\
                                                "    t1.`count`,                                                                 \n"\
                                                "    t1.`price`,                                                                 \n"\
                                                "    t1.`count` * t1.`price` AS 'summ',                                          \n"\
                                                "    t1.`state`,                                                                 \n"\
                                                "    t1.`item_id`                                                                \n"\
                                                "FROM `store_int_reserve` AS t1                                                  \n"\
                                                "LEFT JOIN store_items AS t2                                                     \n"\
                                                "    ON t1.`item_id` = t2.`id`                                                   \n"\
                                                " WHERE (t1.`to_user` = %1) AND ((1 = t1.`state`) OR (2 = t1.`state`))             "\
                                                )\
                                                .arg(parentTab->employeeId()),\
                                                QSqlDatabase::database("connMain")

#define QUERY_SEL_SALARY_PAYMENTS(date1, date2, user)    QString(                                                                   \
                                                "SELECT                                                                          \n"\
                                                "    t1.`id`,                                                                    \n"\
                                                "    t1.`payment_date`,                                                          \n"\
                                                "    t1.`period_from`,                                                           \n"\
                                                "    t1.`period_to`,                                                             \n"\
                                                "    t1.`summ`,                                                                  \n"\
                                                "    t4.`summ` AS 'toBalance',                                                   \n"\
                                                "    t2.`username` AS 'employee',                                                \n"\
                                                "    t3.`username` AS 'issuer',                                                  \n"\
                                                "    t1.`notes`,                                                                 \n"\
                                                "    t1.`type`                                                                   \n"\
                                                "FROM `salary` AS t1                                                             \n"\
                                                "LEFT JOIN `balance` AS t4                                                       \n"\
                                                "    ON t1.`balance_record` = t4.`id`                                            \n"\
                                                "INNER JOIN `users` AS t2                                                        \n"\
                                                "    ON t1.`user_id` = t2.`id`                                                   \n"\
                                                "INNER JOIN `users` AS t3                                                        \n"\
                                                "    ON t1.`from_user` = t3.`id`                                                 \n"\
                                                "WHERE                                                                           \n"\
                                                "    t1.`period_to` >= '%1'                                                      \n"\
                                                "    AND t1.`period_from` <= '%2'                                                \n"\
                                                "    AND t1.`user_id` = %3;                                                      \n"\
                                                )\
                                                .arg(date1)\
                                                .arg(date2)\
                                                .arg(user),\
                                                QSqlDatabase::database("connMain")

#define QUERY_SEL_SALARY_RECEPTED_DEVICES(date1, date2, user)    QString(                                                             \
                                                "SELECT                                                                            \n"\
                                                "    t1.`id`, t1.`Title`, t1.`real_repair_cost`, t1.`in_date`, t3.`pay_device_in`  \n"\
                                                "FROM `workshop` AS t1                                                             \n"\
                                                "LEFT JOIN `users` AS t3                                                           \n"\
                                                "    ON t1.`manager` = t3.`id`                                                     \n"\
                                                "WHERE                                                                             \n"\
                                                "    `in_date` >= '%1'                                                             \n"\
                                                "    AND `in_date` <= '%2'                                                         \n"\
                                                "    AND `manager` = %3                                                              "\
                                                )\
                                                .arg(date1)\
                                                .arg(date2)\
                                                .arg(user),\
                                                QSqlDatabase::database("connMain")

#define QUERY_SEL_SALARY_ISSUED_DEVICES(date1, date2, user)    QString(                                                               \
                                                "SELECT                                                                            \n"\
                                                "    t2.`id`, t2.`Title`, t2.`real_repair_cost`, t2.`out_date`, t4.`pay_device_out`\n"\
                                                "FROM `workshop_issued` AS `t1`                                                    \n"\
                                                "INNER JOIN `workshop` AS `t2`                                                     \n"\
                                                "    ON `t1`.`repair_id` = `t2`.`id`                                               \n"\
                                                "LEFT JOIN `users` AS t4                                                           \n"\
                                                "    ON t1.`employee_id` = t4.`id`                                                 \n"\
                                                "WHERE                                                                             \n"\
                                                "    t1.`created_at` >= '%1'                                                       \n"\
                                                "    AND t1.`created_at` <= '%2'                                                   \n"\
                                                "    AND t1.`employee_id` = %3                                                     \n"\
                                                )\
                                                .arg(date1)\
                                                .arg(date2)\
                                                .arg(user),\
                                                QSqlDatabase::database("connMain")

#define QUERY_SEL_TECH_REPORTS_STATIC           QString(                                                                              \
                                                "SELECT                                                                            \n"\
                                                "  t1.`id`,                                                                        \n"\
                                                "  t1.`num`,                                                                       \n"\
                                                "  t1.`created`,                                                                   \n"\
                                                "  t1.`user`,                                                                      \n"\
                                                "  t1.`company`,                                                                   \n"\
                                                "  IF(t2.`type`,                                                                   \n"\
                                                "     IF(LENGTH(TRIM(t2.`ur_name`)), t2.`ur_name`, t2.`name`),                     \n"\
                                                "     CONCAT_WS(' ', t2.`surname`, t2.`name`, t2.`patronymic`)) AS 'client',       \n"\
                                                "  t1.`device`,                                                                    \n"\
                                                "  t1.`inventory_number`,                                                          \n"\
                                                "  t1.`serial_number`,                                                             \n"\
                                                "  CONVERT(t1.`production_date`, CHAR) AS 'production_date',                       \n"\
                                                "  CONVERT(t1.`purchase_date`, CHAR) AS 'purchase_date',                           \n"\
                                                "  t1.`initial_cost`,                                                              \n"\
                                                "  t1.`residual_cost`,                                                             \n"\
                                                "  t1.`fault`,                                                                     \n"\
                                                "  t1.`diagnostic_result`,                                                         \n"\
                                                "  t1.`conclusion`,                                                                \n"\
                                                "  t1.`notes`,                                                                     \n"\
                                                "  t2.`short_name` AS 'client_short_name'                                          \n"\
                                                "FROM `tech_reports` AS t1                                                         \n"\
                                                "LEFT JOIN `clients` AS t2                                                         \n"\
                                                "  ON t1.`client` = t2.`id`                                                          "\
                                                )

#define QUERY_SEL_PARTS_REQUESTS_NO_GROUPING    QString(                                                                              \
                                                "SELECT                                                                            \n"\
                                                "  NULL AS 'group',                                                                \n"\
                                                "  NULL AS 'sel',                                                                  \n"\
                                                "  '' AS 'count',                                                                  \n"\
                                                "  NULL AS 'id',                                                                   \n"\
                                                "  0 AS 'C'                                                                          "\
                                                )

#define QUERY_SEL_PARTS_REQUESTS_GROUP_CLIENT   QString(                                                                              \
                                                "SELECT                                                                            \n"\
                                                "  NULL AS 'group',                                                                \n"\
                                                "  NULL AS 'sel',                                                                  \n"\
                                                "  '' AS 'count',                                                                  \n"\
                                                "  NULL AS 'id',                                                                   \n"\
                                                "  0 AS 'C'                                                                        \n"\
                                                "UNION ALL                                                                         \n"\
                                                "SELECT                                                                            \n"\
                                                "  IF( t3.`type`,                                                                  \n"\
                                                "    IF( LENGTH(TRIM(t3.`ur_name`)),                                               \n"\
                                                "      t3.`ur_name`,                                                               \n"\
                                                "      t3.`name`                                                                   \n"\
                                                "    ),                                                                            \n"\
                                                "    CONCAT_WS(' ',                                                                \n"\
                                                "              t3.`surname`,                                                       \n"\
                                                "              t3.`name`,                                                          \n"\
                                                "              t3.`patronymic`)                                                    \n"\
                                                "  ) AS 'group',                                                                   \n"\
                                                "  NULL AS 'sel',                                                                  \n"\
                                                "  SUM(1) AS 'count',                                                              \n"\
                                                "  `client` AS 'id',                                                               \n"\
                                                "  1                                                                               \n"\
                                                "FROM                                                                              \n"\
                                                "  `parts_request` AS t1                                                           \n"\
                                                "  LEFT JOIN `parts_request_employees` AS t2                                       \n"\
                                                "    ON t1.`id` = t2.`request`                                                     \n"\
                                                "  LEFT JOIN `clients` AS t3                                                       \n"\
                                                "    ON t1.`client` = t3.`id`                                                      \n"\
                                                "WHERE                                                                             \n"\
                                                "  `client` IS NOT NULL                                                            \n"\
                                                "GROUP BY `client`;                                                                  "\
                                                )

#define QUERY_SEL_PARTS_REQUESTS_GROUP_FIELD(field) QString(                                                                              \
                                                "SELECT                                                                            \n"\
                                                "  NULL AS 'group',                                                                \n"\
                                                "  NULL AS 'sel',                                                                  \n"\
                                                "  '' AS 'count',                                                                  \n"\
                                                "  NULL AS 'id',                                                                   \n"\
                                                "  0 AS 'C'                                                                        \n"\
                                                "UNION ALL                                                                         \n"\
                                                "SELECT                                                                            \n"\
                                                "  `%1` AS 'group',                                                                \n"\
                                                "  NULL AS 'sel',                                                                  \n"\
                                                "  SUM(1) AS 'count',                                                              \n"\
                                                "  `%1` AS 'id',                                                                   \n"\
                                                "  1                                                                               \n"\
                                                "FROM                                                                              \n"\
                                                "  `parts_request` AS t1                                                           \n"\
                                                "  LEFT JOIN `parts_request_employees` AS t2                                       \n"\
                                                "    ON t1.`id` = t2.`request`                                                     \n"\
                                                "WHERE                                                                             \n"\
                                                "  `%1` IS NOT NULL                                                                \n"\
                                                "GROUP BY `%1`;                                                                      "\
                                                ).arg((field))

#define QUERY_SEL_PARTS_REQUESTS_GROUPS_SUPPLIER QString(                                                                                              \
                                                "SELECT                                                                                             \n"\
                                                "  NULL AS 'group',                                                                                 \n"\
                                                "  NULL AS 'sel',                                                                                   \n"\
                                                "  NULL AS 'count',                                                                                 \n"\
                                                "  NULL AS 'id',                                                                                    \n"\
                                                "  0 AS 'C'                                                                                         \n"\
                                                "UNION                                                                                              \n"\
                                                "SELECT DISTINCT                                                                                    \n"\
                                                "  IF(t4.`id` IS NULL,                                                                              \n"\
                                                "     IFNULL(t6.`ur_name`, CONCAT_WS(' ', t6.`surname`, t6.`name`, t6.`patronymic`)),               \n"\
                                                "     IF(t4.`supplier_id` IS NULL,                                                                  \n"\
                                                "        TRIM(TRAILING '/' FROM t4.`supplier_url`),                                                 \n"\
                                                "        IFNULL(t5.`ur_name`, CONCAT_WS(' ', t5.`surname`, t5.`name`, t5.`patronymic`)))) AS 'group',   \n"\
                                                "  SUM(IF(t4.`select` > 0, 1, 0)) AS 'sel',                                                         \n"\
                                                "  IF(t1.`dealer` IS NOT NULL OR t4.`supplier_id` IS NOT NULL OR t4.`supplier_url` IS NOT NULL, COUNT(DISTINCT t1.`id`), NULL) AS 'count',  \n"\
                                                "  IF(t4.`supplier_id` IS NOT NULL OR t4.`supplier_url` IS NOT NULL, t4.`supplier_id`, t1.`dealer`) AS 'id',    \n"\
                                                "  IF(t1.`dealer` IS NOT NULL OR t4.`supplier_id` IS NOT NULL OR t4.`supplier_url` IS NOT NULL, 1, 0) AS 'C'  \n"\
                                                "FROM                                                                                               \n"\
                                                "  `parts_request` AS t1                                                                            \n"\
                                                "  LEFT JOIN `parts_request_employees` AS t2                                                        \n"\
                                                "    ON t1.`id` = t2.`request`                                                                      \n"\
                                                "  LEFT JOIN `parts_request_suppliers` AS t4                                                        \n"\
                                                "    ON t1.`id` = t4.`request_id`                                                                   \n"\
                                                "  LEFT JOIN `clients` AS t5                                                                        \n"\
                                                "    ON t4.`supplier_id` = t5.`id`                                                                  \n"\
                                                "  LEFT JOIN `clients` AS t6                                                                        \n"\
                                                "    ON t1.`dealer` = t6.`id`                                                                       \n"\
                                                "WHERE                                                                                              \n"\
                                                "  (t1.`dealer` IS NOT NULL OR t4.`supplier_id` IS NOT NULL OR t4.`supplier_url` IS NOT NULL)       \n"\
                                                "GROUP BY                                                                                           \n"\
                                                "  `group`;                                                                                           "\
                                                )

#define QUERY_SEL_PART_REQUEST(requestId)       QString(                                                                                               \
                                                "SELECT                                                                                             \n"\
                                                "  `id`,                                                                                            \n"\
                                                "  `request_time`,                                                                                  \n"\
                                                "  `from_user`,                                                                                     \n"\
                                                "  `repair`,                                                                                        \n"\
                                                "  `client`,                                                                                        \n"\
                                                "  `state`,                                                                                         \n"\
                                                "  `end_date`,                                                                                      \n"\
                                                "  `plan_end_date`,                                                                                 \n"\
                                                "  `summ`,                                                                                          \n"\
                                                "  `tracking`,                                                                                      \n"\
                                                "  `item_id`,                                                                                       \n"\
                                                "  `item_name`,                                                                                     \n"\
                                                "  `notes`,                                                                                         \n"\
                                                "  `url`,                                                                                           \n"\
                                                "  `pririty`,                                                                                       \n"\
                                                "  `count`,                                                                                         \n"\
                                                "  `dealer`                                                                                         \n"\
                                                "FROM `parts_request`                                                                               \n"\
                                                "WHERE `id` = %1;                                                                                     "\
                                                ).arg((requestId))

#define QUERY_SEL_PARTS_REQUESTS_STATIC         QString(                                                                                               \
                                                "SELECT                                                                                             \n"\
                                                "  0 AS 'select',                                                                                   \n"\
                                                "  t1.`id`,                                                                                         \n"\
                                                "  t1.`request_time`,                                                                               \n"\
                                                "  t1.`from_user`,                                                                                  \n"\
                                                "  IF(t3.`type`, IF(LENGTH(TRIM(t3.`ur_name`)), t3.`ur_name`, t3.`name`), CONCAT_WS(' ', t3.`surname`, t3.`name`, t3.`patronymic`)) AS 'client',\n"\
                                                "  t1.`item_name`,                                                                                  \n"\
                                                "  t1.`count`,                                                                                      \n"\
                                                "  IF(t6.`type`, IF(LENGTH(TRIM(t6.`ur_name`)), t6.`ur_name`, t6.`name`), CONCAT_WS(' ', t6.`surname`, t6.`name`, t6.`patronymic`)) AS 'dealer',\n"\
                                                "  t1.`repair`,                                                                                     \n"\
                                                "  t1.`state`,                                                                                      \n"\
                                                "  t1.`tracking`,                                                                                   \n"\
                                                "  t1.`pririty`,                                                                                    \n"\
                                                "  t1.`state`,                                                                                      \n"\
                                                "  t1.`end_date`,                                                                                   \n"\
                                                "  t1.`plan_end_date`,                                                                              \n"\
                                                "  t1.`summ`,                                                                                       \n"\
                                                "  t1.`item_id`,                                                                                    \n"\
                                                "  t1.`notes`,                                                                                      \n"\
                                                "  t1.`url`,                                                                                        \n"\
                                                "  t3.`short_name` AS 'client_short_name'                                                           \n"\
                                                "FROM                                                                                               \n"\
                                                "  `parts_request` AS t1                                                                            \n"\
                                                "  LEFT JOIN `parts_request_employees` AS t2                                                        \n"\
                                                "    ON t1.`id` = t2.`request`                                                                      \n"\
                                                "  LEFT JOIN `clients` AS t3                                                                        \n"\
                                                "    ON t1.`client` = t3.`id`                                                                       \n"\
                                                "  LEFT JOIN `parts_request_suppliers` AS t4                                                        \n"\
                                                "    ON t4.`request_id` = t1.`id`                                                                   \n"\
                                                "  LEFT JOIN `clients` AS t5                                                                        \n"\
                                                "    ON t4.`supplier_id` = t5.`id`                                                                  \n"\
                                                "  LEFT JOIN `clients` AS t6                                                                        \n"\
                                                "    ON t1.`dealer` = t6.`id`                                                                         "\
                                                )

#define QUERY_SEL_PRT_RQST_SUPPLIERS(rqstId) QString("SELECT `id`, `request_id`, `supplier_id`, `supplier_url`, `item_url`, `moq`, `price`, `select`, `notes` FROM `parts_request_suppliers` WHERE `request_id` = %1;").arg((rqstId))

#define QUERY_UPD_PRT_RQST_SUPPLIERS_SEL(id, rqst)   QString("UPDATE `parts_request_suppliers` SET `select` = IF(`id` = %1, 1, 0) WHERE `request_id` = %2;").arg((id)).arg((rqst))

#define QUERY_UPD_CLIENT_PURCHASES(id, num)      QString("UPDATE `clients` SET `purchases`=`purchases`+(%2) WHERE `id` = %1;").arg((id)).arg((num))
#define QUERY_UPD_CLIENT_REPAIRS(id)      QString("UPDATE `clients` SET `repairs`=`repairs`+1 WHERE `id` = %1;").arg((id))

#define QUERY_INS_SCHEMAVERSIONS(scriptName)       QString("INSERT INTO `schemaversions` (ScriptName, Applied) VALUES ('%1',UTC_TIMESTAMP());").arg(scriptName)
#define QUERY_UPD_APP_VER(appVer)                  QString("UPDATE `config` SET `version_snap`='%1' WHERE `id`=1;").arg(appVer)
#define QUERY_UPD_DOC_TEMPL_DATA(doc, data, hash)  QString("UPDATE `doc_templates_snap` SET `data` = UNHEX('%1'), `checksum` = UNHEX('%2') WHERE `name` = '%3';").arg((data)).arg((hash)).arg((doc))

#endif // COM_SQL_QUERIES_H

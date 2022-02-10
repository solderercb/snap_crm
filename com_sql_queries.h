#ifndef COM_SQL_QUERIES_H
#define COM_SQL_QUERIES_H

#include "appver.h"

#define QUERY_SEL_CLIENT(id)                QString("SELECT  `id`,  `name`,  `surname`,  `patronymic`,  `agent_phone_mask`,  `agent2_phone_mask`,  `address`,  `post_index`,  `type`,  `memorial`,  `notes`,  `prefer_cashless`,  `visit_source`,  `ur_name`,  `email`,  `balance`,  `price_col`,  `repairs`,  `purchases` FROM `clients` WHERE `id` = %1 AND `state` = 1").arg((id))
#define QUERY_SEL_CLIENT_AD_TYPES           QString("SELECT '' AS 'name', '' AS 'id' UNION ALL (SELECT `name`, `id` FROM visit_sources WHERE `enabled` ORDER BY `position` ASC);")
#define QUERY_SEL_CLIENT_MATCH              QString("SELECT t1.`id`, CONCAT_WS(' ', t1.`surname`, t1.`name`, t1.`patronymic`) AS 'FIO', t1.`balance`, t1.`repairs`, t1.`purchases`, IF(t1.`type` = 1, 'Ю', '') AS 'type', GROUP_CONCAT(IFNULL(t2.`phone`, '') ORDER BY t2.`type` DESC, t2.`id` DESC SEPARATOR '\r\n')  AS 'phone' FROM `clients` AS t1 LEFT JOIN `tel` AS t2 ON t1.`id` = t2.`customer` WHERE `state` = 1 %1 GROUP BY t1.`id`;")
#define QUERY_SEL_CLIENT_PHONES(id)         QString("SELECT `phone`, `mask` FROM `tel` WHERE `customer` = " + QString::number((id)) + " ORDER BY `type` DESC, `id` DESC LIMIT 2")
#define QUERY_SEL_USER_DATA(username)       QString("SELECT `users`.`id`, GROUP_CONCAT(`role_id`) AS 'roles', `name`,  `surname`,  `patronymic`,  `office`,  `def_office`,  `def_store`,  `def_item_state`,  `def_employee`,  `def_status`,  `def_ws_filter`,  `salary_rate`,  `pay_day`,  `pay_day_off`,  `pay_repair`,  `pay_repair_quick`,  `pay_sale`,  `pay_repair_q_sale`,  `pay_cartridge_refill`,  `pay_device_in`,  `pay_device_out`,  `pay_4_sale_in_repair`,  `row_color`,  `ge_highlight_color`,  `color_label_ws`,  `workspace_mode`,  `preview_before_print`,  `new_rep_doc_copies`,  `auto_refresh_workspace`,  `refresh_time`,  `xls_c1`,  `xls_c2`,  `xls_c3`,  `xls_c4`,  `xls_c5`,  `xls_c6`,  `xls_c7`,  `xls_c8`,  `xls_c9`,  `xls_c10`,  `xls_c11`,  `xls_c12`,  `xls_c13`,  `xls_c14`,  `xls_c15`,  `display_out`,  `display_complete`,  `new_on_top`,  `issued_color`, `fields_cfg`,  `save_state_on_close`,  `group_store_items`,  `track_activity`,  `card_on_call`,  `inn`,  `inform_comment`,  `inform_status`,  `kkt`,  `pinpad`,  `advance_disable`,  `salary_disable`,  `kkm_pass`,  `prefer_regular`,  `fontsize`,  `rowheight`,  `animation` FROM `users` LEFT JOIN `roles_users` ON `users`.`id` = `roles_users`.`user_id` WHERE `username` = '%1' GROUP BY `users`.`id`  LIMIT 1;").arg((username))
#define QUERY_SEL_PERMISSIONS(roles)        QString("SELECT `permission_id` FROM `permissions_roles` WHERE `role_id` IN (%1)").arg((roles))
#define QUERY_SEL_COMMON_SETTINGS           QString("SELECT  `id`,  `default_works_warranty`,  `default_items_warranty`,  `currency`,  `configcol`,  `email_config`,  `qs_print_pko`,  `qs_print_rko`,  `qs_print_pn`,  `qs_print_rn`,  `print_rep_stickers`,  `rep_stickers_copy`,  `rep_auto_company`,  `config_modified`,  `close_clients`,  `aster_host`,  `aster_port`,  `aster_login`,  `aster_password`,  `aster_web_port`,  `time_zone`,  `realizator_enable`,  `online_store`,  `online_store_api`,  `online_store_key`,  `classic_kassa`,  `default_items_used_warranty`,  `default_items_rep_warranty`,  `default_items_razb_warranty`,  `default_items_other_warranty`,  `images_width`,  `item_img_limit`,  `rep_img_limit`,  `default_reserve_days`,  `it_vis_opt`,  `it_vis_opt2`,  `it_vis_opt3`,  `it_vis_rozn`,  `it_vis_price_for_sc`,  `give2user_time`,  `diag_accept_time`,  `salary_day`,  `parts_included`,  `visit_source_force`,  `is_patronymic_required`,  `is_photo_required`,  `is_photo_out_req`,  `is_sn_req`,  `is_cartridge_sn_req`,  `is_reason_req`,  `address_required`,  `email_required`,  `phone_required`,  `diag_required`,  `is_master_set_on_new`,  `print_warranty`,  `print_works`,  `print_diagnostic`,  `print_reject`,  `print_check`,  `rko_on_pn`,  `card_close_time`,  `phone_mask1`,  `phone_mask2`,  `auto_switch_layout`,  `rep_price_by_manager`,  `video_codec`,  `print_new_repair_report`,  `statuses`,  `sms_config`,  `debt_rep_2_salary`,  `exchange_type`,  `exchange_source`,  `exchange_rate`,  `history_by_sn`,  `clients_are_dealers`,  `regular_customer_step`,  `backup_enable`,  `backup_images`,  `backup_time`,  `manual_maker`,  `TimeOfWork`,  `cartridge_enable`,  `vw_enable`,  `termsControl`,  `version` FROM `config` WHERE `id` = 1;")
#define QUERY_SEL_COMPANIES                 QString("SELECT  `name`,  `id`,  `type`,  `inn`,  `kpp`,  `ogrn`,  `ur_address`,  `site`,  `email`,  `logo`,  `banks`,  `is_default`,  `status`,  `director`,  `accountant`,  `tax_form`,  `description` FROM `companies` WHERE `id` = 1;")
#define QUERY_SEL_OFFICES(id)               QString("SELECT  t1.`name`, CONCAT(t1.`name`, ' (', t2.`name`, ')') AS 'name_full',  t1.`id`,  t1.`state`,  `address`,  `phone`,  t1.`logo` AS 'off_logo',  t2.`logo` AS 'comp_logo',  `administrator`,  `created`,  `phone2`,  `default_company`,  `card_payment`,  `use_boxes`,  `paint_repairs`,  `warranty_sn` FROM `offices` AS t1 LEFT JOIN `companies` AS t2 ON t1.`default_company` = t2.`id` WHERE t1.`state` = 1 AND `default_company`  = %1;").arg((id))
#define QUERY_SEL_WAREHOUSES(office)        QString("SELECT `name`, `id`, `type`, `description`, `sub_type`, `it_vis_pn`, `it_vis_notes`, `it_vis_description`, `it_vis_sn`, `it_vis_barcode`, `it_vis_photo`, `it_vis_warranty`, `it_vis_warranty_dealer`, `active`FROM `stores` WHERE `office` = %1;").arg((office))
#define QUERY_SEL_USERS                     QString("SELECT `id`,  `username`,  `name`,  `surname`,  `patronymic`, `office`, `inform_comment`, `inform_status` FROM `users` WHERE `status` = 1;")
#define QUERY_SEL_MANAGERS                  QString("SELECT `username`, t1.`id`, `name`,  `surname`,  `patronymic`, `office`, `inform_comment`, `inform_status` FROM `users` AS t1 LEFT JOIN `roles_users` AS t2 ON t1.`id` = t2.`user_id` WHERE t1.`state` = 1 AND t2.role_id IN (3, 6) GROUP BY t1.`id`;")
#define QUERY_SEL_ENGINEERS                 QString("SELECT `username`, t1.`id`, `name`,  `surname`,  `patronymic`, `office`, `inform_comment`, `inform_status` FROM `users` AS t1 LEFT JOIN `roles_users` AS t2 ON t1.`id` = t2.`user_id` WHERE t1.`state` = 1 AND t2.role_id IN (2, 5) GROUP BY t1.`id`;")
#define QUERY_SEL_ITEM_BOXES(warehouse)     QString("SELECT `name`, `id`, `places`, `color` FROM `boxes` WHERE `store_id` = 1 AND `non_items` = 0 ORDER BY `name`;").arg((warehouse))
#define QUERY_SEL_REPAIR_BOXES              QString("SELECT `name`, `id`,`places`, `color` FROM `boxes` WHERE `non_items` = 1 ORDER BY `name`;")
#define QUERY_SEL_PAYMENT_SYSTEMS           QString("SELECT `name`, `system_id`, `system_data`, `id` FROM `payment_systems` WHERE `is_enable` = 1 ORDER BY `name`;")
#define QUERY_SEL_DEVICE(id)                QString("SELECT  `id`,  `type`,  `maker`,  `model`, `serial_number`,  `client` FROM `workshop` WHERE `id` = %1").arg((id))
#define QUERY_SEL_DEVICES                   QString("SELECT `name`, `id`, `company_list` FROM `devices` WHERE `enable` = 1 AND `refill` = 0 ORDER BY `position`;")
#define QUERY_SEL_DEVICE_MAKERS(device)     QString("SELECT `name`, `id` FROM `device_makers` WHERE `id` IN (%1);").arg((device))
#define QUERY_SEL_DEVICE_MODELS             QString("SELECT `name`, `id` FROM `device_models` WHERE `device` = %1 AND `maker` = %2;")
#define QUERY_SEL_DEVICE_FAULTS(device)     QString("\
                                                SELECT '' AS 'name'\
                                                UNION ALL\
                                                (SELECT\
                                                  TRIM(SUBSTRING_INDEX(SUBSTRING_INDEX(t1.`fault_list`,',',enumerator.`number`),',',1)) AS 'name'\
                                                FROM (SELECT `fault_list` FROM `devices` WHERE `id` = %1) AS t1\
                                                JOIN enumerator\
                                                ON\
                                                  (LENGTH(REPLACE(t1.`fault_list`, ',', ''))-LENGTH(t1.`fault_list`) <= enumerator.`number`+1));\
                                                ").arg((device))
#define QUERY_SEL_DEVICE_SET(device)        QString("\
                                                SELECT '' AS 'name'\
                                                UNION ALL\
                                                (SELECT\
                                                  TRIM(SUBSTRING_INDEX(SUBSTRING_INDEX(t1.`complect_list`,',',enumerator.`number`),',',1)) AS 'name'\
                                                FROM (SELECT `complect_list` FROM `devices` WHERE `id` = %1) AS t1\
                                                JOIN enumerator\
                                                ON\
                                                  (LENGTH(REPLACE(t1.`complect_list`, ',', ''))-LENGTH(t1.`complect_list`) <= enumerator.`number`+1));\
                                            ").arg((device))
#define QUERY_SEL_DEVICE_EXTERIOR(device)   QString("\
                                                SELECT '' AS 'name'\
                                                UNION ALL\
                                                (SELECT\
                                                  TRIM(SUBSTRING_INDEX(SUBSTRING_INDEX(t1.`look_list`,',',enumerator.`number`),',',1)) AS 'name'\
                                                FROM (SELECT `look_list` FROM `devices` WHERE `id` = %1) AS t1\
                                                JOIN enumerator\
                                                ON\
                                                  (LENGTH(REPLACE(t1.`look_list`, ',', ''))-LENGTH(t1.`look_list`) <= enumerator.`number`+1));\
                                                ").arg((device))
#define QUERY_SEL_DEVICE_ADD_FIELDS(device) QString("SELECT `name`, `def_values`, `type`, `id`,  `required`,  `printable`  FROM `fields` WHERE `_f` = 0 AND FIND_IN_SET(%1,`devices`) AND `archive` = 0 ORDER BY `id`;").arg((device))
#define QUERY_SEL_DEVICE_MATCH(text)        QString("\
                                                SELECT\
                                                  t1.`id`,\
                                                  CONCAT_WS(' ', t2.`name`,  t3.`name`,  t4.`name`) AS 'device',\
                                                  `fault`,\
                                                  `serial_number`,\
                                                  CONCAT_WS(' ', t5.surname, t5.name, t5.patronymic) AS 'client',\
                                                  t1.`type`,\
                                                  t1.`maker`,\
                                                  t1.`model`,\
                                                  t1.`client` AS 'client_id'\
                                                FROM `workshop` AS t1 LEFT JOIN `devices` AS t2 ON t1.`type` = t2.`id` LEFT JOIN `device_makers` AS t3 ON t1.maker = t3.`id` LEFT JOIN `device_models` AS t4 ON t1.model = t4.`id` LEFT JOIN `clients` AS t5 ON t1.`client` = t5.`id` WHERE LCASE(`serial_number`) REGEXP LCASE('%1') ORDER BY `id` DESC;\
                                                ").arg((text))
#define QUERY_SEL_LAST_INSERT_ID            QString("SELECT LAST_INSERT_ID();")
#define QUERY_BEGIN                         QString("BEGIN;")
#define QUERY_COMMIT                        QString("COMMIT;")
#define QUERY_ROLLBACK                      QString("ROLLBACK;")
#define QUERY_INS_DEVICE_MODEL              QString("INSERT INTO `device_models` (`name`, `position`, `maker`, `device`) VALUES ('%1', %2, %3, %4);)")


#endif // COM_SQL_QUERIES_H

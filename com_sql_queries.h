#ifndef COM_SQL_QUERIES_H
#define COM_SQL_QUERIES_H

#include "appver.h"

#define QUERY_SEL_CLIENT(id)                QString("SELECT  `id`,  `name`,  `surname`,  `patronymic`,  `agent_phone_mask`,  `agent2_phone_mask`,  `address`,  `post_index`,  `type`,  `memorial`,  `notes`,  `prefer_cashless`,  `visit_source`,  `ur_name`,  `email`,  `balance`,  `price_col`,  `repairs`,  `purchases` FROM `clients` WHERE `id` = %1 AND `state` = 1").arg((id))
#define QUERY_SEL_CLIENT_AD_TYPES           QString("SELECT `name`, `id` FROM visit_sources WHERE `enabled` ORDER BY `position` ASC;")
#define QUERY_SEL_CLIENT_MATCH              QString("SELECT t1.`id`, CONCAT_WS(' ', t1.`surname`, t1.`name`, t1.`patronymic`) AS 'FIO', t1.`balance`, t1.`repairs`, t1.`purchases`, IF(t1.`type` = 1, 'Ю', '') AS 'type', GROUP_CONCAT(IFNULL(t2.`phone`, '') ORDER BY t2.`type` DESC, t2.`id` DESC SEPARATOR '\r\n')  AS 'phone' FROM `clients` AS t1 LEFT JOIN `tel` AS t2 ON t1.`id` = t2.`customer` WHERE `state` = 1 %1 GROUP BY t1.`id`;")
#define QUERY_SEL_CLIENT_PHONES(id)         QString("SELECT `phone`, `mask` FROM `tel` WHERE `customer` = " + QString::number((id)) + " ORDER BY `type` DESC, `id` DESC LIMIT 2")
#define QUERY_SEL_USER_DATA(username)       QString("SELECT `users`.`id`, GROUP_CONCAT(`role_id`) AS 'roles', `name`,  `surname`,  `patronymic`, CONCAT(IF(users.`surname` IS NULL OR users.`surname` = '', '', CONCAT(users.`surname`, ' ')), IF(users.`name` IS NULL OR users.`name` = '', '', CONCAT(LEFT(users.`name`, 1), '. ')), IF(users.`patronymic` IS NULL OR users.`patronymic` = '', '', CONCAT(LEFT(users.`patronymic`, 1), '.'))) AS 'fio', `office`,  `def_office`,  `def_store`,  `def_item_state`,  `def_employee`,  `def_status`,  `def_ws_filter`,  `salary_rate`,  `pay_day`,  `pay_day_off`,  `pay_repair`,  `pay_repair_quick`,  `pay_sale`,  `pay_repair_q_sale`,  `pay_cartridge_refill`,  `pay_device_in`,  `pay_device_out`,  `pay_4_sale_in_repair`,  `row_color`,  `ge_highlight_color`,  `color_label_ws`,  `workspace_mode`,  `preview_before_print`,  `new_rep_doc_copies`,  `auto_refresh_workspace`,  `refresh_time`,  `xls_c1`,  `xls_c2`,  `xls_c3`,  `xls_c4`,  `xls_c5`,  `xls_c6`,  `xls_c7`,  `xls_c8`,  `xls_c9`,  `xls_c10`,  `xls_c11`,  `xls_c12`,  `xls_c13`,  `xls_c14`,  `xls_c15`,  `display_out`,  `display_complete`,  `new_on_top`,  `issued_color`, `fields_cfg`,  `save_state_on_close`,  `group_store_items`,  `track_activity`,  `card_on_call`,  `inn`,  `inform_comment`,  `inform_status`,  `kkt`,  `pinpad`,  `advance_disable`,  `salary_disable`,  `kkm_pass`,  `prefer_regular`,  `fontsize`,  `rowheight`,  `animation` FROM `users` LEFT JOIN `roles_users` ON `users`.`id` = `roles_users`.`user_id` WHERE `username` = '%1' GROUP BY `users`.`id`  LIMIT 1;").arg((username))
#define QUERY_SEL_PERMISSIONS(roles)        QString("SELECT `permission_id` FROM `permissions_roles` WHERE `role_id` IN (%1)").arg((roles))
#define QUERY_SEL_COMMON_SETTINGS           QString("SELECT  `id`,  `default_works_warranty`,  `default_items_warranty`,  `currency`,  `configcol`,  `email_config`,  `qs_print_pko`,  `qs_print_rko`,  `qs_print_pn`,  `qs_print_rn`,  `print_rep_stickers`,  `rep_stickers_copy`,  `rep_auto_company`,  `config_modified`,  `close_clients`,  `aster_host`,  `aster_port`,  `aster_login`,  `aster_password`,  `aster_web_port`,  `time_zone`,  `realizator_enable`,  `online_store`,  `online_store_api`,  `online_store_key`,  `classic_kassa`,  `default_items_used_warranty`,  `default_items_rep_warranty`,  `default_items_razb_warranty`,  `default_items_other_warranty`,  `images_width`,  `item_img_limit`,  `rep_img_limit`,  `default_reserve_days`,  `it_vis_opt`,  `it_vis_opt2`,  `it_vis_opt3`,  `it_vis_rozn`,  `it_vis_price_for_sc`,  `give2user_time`,  `diag_accept_time`,  `salary_day`,  `parts_included`,  `visit_source_force`,  `is_patronymic_required`,  `is_photo_required`,  `is_photo_out_req`,  `is_sn_req`,  `is_cartridge_sn_req`,  `is_reason_req`,  `address_required`,  `email_required`,  `phone_required`,  `diag_required`,  `is_master_set_on_new`,  `print_warranty`,  `print_works`,  `print_diagnostic`,  `print_reject`,  `print_check`,  `rko_on_pn`,  `card_close_time`,  `phone_mask1`,  `phone_mask2`,  `auto_switch_layout`,  `rep_price_by_manager`,  `video_codec`,  `print_new_repair_report`,  `statuses`,  `sms_config`,  `debt_rep_2_salary`,  `exchange_type`,  `exchange_source`,  `exchange_rate`,  `history_by_sn`,  `clients_are_dealers`,  `regular_customer_step`,  `backup_enable`,  `backup_images`,  `backup_time`,  `manual_maker`,  `TimeOfWork`,  `cartridge_enable`,  `vw_enable`,  `termsControl`,  `version` FROM `config` WHERE `id` = 1;")
#define QUERY_SEL_COMMON_SETTINGS2          QString("SELECT  `name`,  `value` FROM `settings`")
#define QUERY_SEL_COMPANIES                 QString("SELECT  `name`,  `id`,  `type`,  `inn`,  `kpp`,  `ogrn`,  `ur_address`,  `site`,  `email`,  `logo`,  `banks`,  `is_default`,  `status`,  `director`,  `accountant`,  `tax_form`,  `description` FROM `companies` WHERE `id` = 1;")
#define QUERY_SEL_OFFICES(id)               QString("SELECT  t1.`name`, CONCAT(t1.`name`, ' (', t2.`name`, ')') AS 'name_full',  t1.`id`,  t1.`state`,  `address`,  `phone`,  t1.`logo` AS 'logo',  t2.`logo` AS 'companyLogo',  `administrator`,  `created`,  `phone2`,  `default_company`,  `card_payment`,  `use_boxes`,  `paint_repairs`,  `warranty_sn` FROM `offices` AS t1 LEFT JOIN `companies` AS t2 ON t1.`default_company` = t2.`id` WHERE t1.`state` = 1 AND `default_company`  = %1;").arg((id))
#define QUERY_SEL_WAREHOUSES(office)        QString("SELECT `name`, `id`, `type`, `description`, `sub_type`, `it_vis_pn`, `it_vis_notes`, `it_vis_description`, `it_vis_sn`, `it_vis_barcode`, `it_vis_photo`, `it_vis_warranty`, `it_vis_warranty_dealer`, `active`FROM `stores` WHERE `office` = %1;").arg((office))
#define QUERY_SEL_ALL_USERS                 QString("SELECT `username`, `id`,  `name`,  `surname`,  `patronymic`, `office`, `inform_comment`, `inform_status` FROM `users` WHERE `is_bot` = 0;")
#define QUERY_SEL_USERS                     QString("SELECT `username`, `id`,  `name`,  `surname`,  `patronymic`, `office`, `inform_comment`, `inform_status` FROM `users` WHERE `state` = 1 AND  `is_bot` = 0;")
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
#define QUERY_SEL_DEVICE_ADD_FIELDS(device) QString("SELECT `name`, REPLACE(`def_values`,'\r','') AS 'def_values', `type`, `id`,  `required`,  `printable`  FROM `fields` WHERE `_f` = 0 AND FIND_IN_SET(%1,`devices`) AND `archive` = 0 ORDER BY `id`;").arg((device))
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

#define QUERY_SEL_CLIENTS_STATIC            QString("\
                                                SELECT\
                                                  t1.`id`,\
                                                  CONCAT_WS(' ', t1.`surname`, t1.`name`, t1.`patronymic`) AS 'FIO',\
                                                  t1.`balance`,\
                                                  t1.`repairs`,\
                                                  t1.`purchases`,\
                                                  IF(t1.`type` = 1, 'Ю', '') AS 'type',\
                                                  IFNULL(t2.`phone`, '') AS 'phone'\
                                                FROM `clients` AS t1\
                                                LEFT JOIN `tel` AS t2\
                                                ON t1.`id` = t2.`customer`\
                                                  AND t2.`type` = 1")   // Здесь не должно быть точки с запятой!

#define QUERY_SEL_WORKSHOP_STATIC           QString("\
                                                SELECT\
                                                   CONCAT_WS(' ', IF(`is_warranty`, 'Г', ''), IF(`is_repeat`, 'П', ''), IF(`express_repair`, 'С', ''), IF(`informed_status`, '*', '')) AS 'marks',\
                                                   t1.`id`,\
                                                   `Title`,\
                                                   CONCAT_WS(' ', t5.surname, t5.name, t5.patronymic) AS 'client',\
                                                   CONCAT_WS(' ', t2.`name`,  t3.`name`,  t4.`name`) AS 'device',\
                                                   `serial_number`,\
                                                   `office`,\
                                                   `manager`,\
                                                   `master`,\
                                                   `in_date`,\
                                                   t1.`state`,\
                                                   t1.`state`,\
                                                   `new_state`,\
                                                   `user_lock`,\
                                                   `lock_datetime`,\
                                                   `quick_repair`,\
                                                   `box`,\
                                                   `repair_cost`,\
                                                   `fault`,\
                                                   `thirs_party_sc`,\
                                                   `last_save`,\
                                                   `last_status_changed`,\
                                                   `warranty_days`,\
                                                   `color`,\
                                                   `early`,\
                                                   `ext_early`,\
                                                   `issued_msg`,\
                                                   `vendor_id`,\
                                                   `termsControl`,\
                                                   `Hidden`\
                                                FROM `workshop` AS t1\
                                                LEFT JOIN `devices` AS t2\
                                                   ON t1.`type` = t2.`id`\
                                                LEFT JOIN `device_makers` AS t3\
                                                   ON t1.maker = t3.`id`\
                                                LEFT JOIN `device_models` AS t4\
                                                   ON t1.model = t4.`id`\
                                                LEFT JOIN `clients` AS t5\
                                                   ON t1.`client` = t5.`id`")   // Здесь не должно быть точки с запятой!

#define QUERY_SEL_REPAIR_DATA(id)           QString("SELECT t2.PrepaidTypeStr, workshop.* FROM workshop LEFT JOIN (SELECT \"полная предоплата\" AS 'PrepaidTypeStr', 0 AS 'id' UNION SELECT \"за детали\", 1 UNION SELECT \"за часть стоимости деталей\", 2 UNION SELECT \"за часть стоимости работ\", 3 UNION SELECT \"за диагностику\", 4 ) AS `t2` ON workshop.prepaid_type = t2.`id` WHERE workshop.`id` = %1;").arg((id))
#define QUERY_SEL_REPAIR_COMMENTS(id)       QString("SELECT `created`, `user`, `text` FROM `comments` WHERE `remont` = %1 ORDER BY `id` DESC;").arg((id)), QSqlDatabase::database("connMain")
#define QUERY_SEL_REPAIR_WORKS_AND_PARTS(id) QString("SELECT 'X  +' AS '*', t1.`name`,t1.`count`,ROUND(t1.`price`, 2) AS 'price', t1.`count`*ROUND(t1.`price`, 2) AS 'summ',t1.`warranty`,t1.`user`,'' AS 'item_sn',t1.`id`, t1.`id` AS 'work_id', '' AS 'item_rsrv_id' FROM `works` AS t1 WHERE `repair` = %1\
                                                        UNION ALL\
                                                        SELECT '   X', t2.`name`,t2.`count`,ROUND(t2.`price`, 2),t2.`count`*ROUND(t2.`price`, 2),t2.`warranty`,t2.`to_user`, t2.`sn`,t2.`id`,t2.`work_id`, t2.`id` FROM `store_int_reserve` AS t2 WHERE t2.work_id IN (SELECT `id` FROM `works` WHERE `repair` = %1) ORDER BY `work_id` ASC, `item_rsrv_id` ASC;").arg((id)), QSqlDatabase::database("connMain")

/**************** SELECT queries for data models for reports ******************/
#define QUERY_SEL_REPAIR_RPRT(R)            QString("SELECT t2.PrepaidTypeStr, workshop.* FROM workshop LEFT JOIN (SELECT \"полная предоплата\" AS 'PrepaidTypeStr', 0 AS 'id' UNION SELECT \"за детали\", 1 UNION SELECT \"за часть стоимости деталей\", 2 UNION SELECT \"за часть стоимости работ\", 3 UNION SELECT \"за диагностику\", 4 ) AS `t2` ON workshop.prepaid_type = t2.`id` WHERE workshop.`id` = %1;").arg(R), QSqlDatabase::database("connMain")
#define QUERY_SEL_CLIENT_RPRT(C)            QString("SELECT t1.*, GROUP_CONCAT(t2.phone ORDER BY t2.`type` DESC SEPARATOR ',') AS 'phone', IF(t1.`type` = 1, t1.`ur_name`, CONCAT(t1.`surname`, ' ', t1.`name`, ' ', t1.`patronymic`)) AS 'FioOrUrName' FROM clients as t1 LEFT JOIN tel AS t2 ON t1.id = t2.customer WHERE t1.`id` = %1 GROUP BY t1.`id`;").arg(C), QSqlDatabase::database("connMain")
#define QUERY_SEL_REP_FIELDS_RPRT(R)        QString("(SELECT 'Предоплата' AS 'name', CONCAT(ROUND(`prepaid_summ`, 0), '$D{config.currency}') AS 'value', `prepaid_type` AS 'comment' FROM workshop WHERE `id` = %1 AND `is_prepaid` = 1)\
                                                      UNION ALL\
                                                      (SELECT 'Ориентировочная стоимость ремонта', CONCAT(ROUND(`pre_agreed_amount`, 0), '$D{config.currency}'), '' FROM workshop WHERE `id` = %1 AND `is_pre_agreed` = 1)\
                                                      UNION ALL\
                                                      (SELECT '', 'Уст-во было в другом СЦ', '' FROM workshop WHERE `id` = %1 AND `thirs_party_sc` = 1)\
                                                      UNION ALL\
                                                      (SELECT t2.`name`, t1.`value`, '' AS 'comment' FROM `field_values` AS t1 LEFT JOIN `fields` AS t2 ON t1.`field_id` = t2.`id` WHERE t2.`printable` = 1 AND t2.`archive` = 0 AND t1.`repair_id` = %1 ORDER BY t1.`field_id` ASC);\
                                                      ").arg((R)), QSqlDatabase::database("connMain")
//#define QUERY_SEL__RPRT()  QString("").arg(()), QSqlDatabase::database("connMain")
//#define QUERY_SEL__RPRT()  QString("").arg(()), QSqlDatabase::database("connMain")
//#define QUERY_SEL__RPRT()  QString("").arg(()), QSqlDatabase::database("connMain")
//#define QUERY_SEL__RPRT()  QString("").arg(()), QSqlDatabase::database("connMain")

/**************** Queries for data insertion ******************/
#define QUERY_SEL_LAST_INSERT_ID            QString("SELECT LAST_INSERT_ID();")

// ACHTUNG! ACHTUNG! если в выражениях ниже не закомментирован вызов функции qDebug(), а выражение используется в теле условного оператора БЕЗ ЗАКЛЮЧЕНИЯ в {}, то работа программы будет неправильной
#define QUERY_EXEC(obj,flag)                /*qDebug() << "(!!!) nDBErr=" << (flag) << ". Executing query...";*/ if ((flag)) (flag) = (obj)->exec

// верификация записанных в БД данных; производится примитивно, путём сравнения записанных данных с данными, которые были переданы в запросе;
// если данные совпадают, то сервер должен вернуть 55AA55AAh (1437226410d), иначе 00h; ХЗ, может такое контрольное значение избыточно, ну а вдруг;
// далее, если контрольное число совпадает, flag устанавливаем в 1, иначе — 0.
#define QUERY_EXEC_VRFY(obj,flag)           /*qDebug() << "(!!!) nDBErr=" << (flag) << ". Checking integrity...";*/ if ((flag)){\
                                                (obj)->first();\
                                                /*qDebug() << "Integrity magic number: " << (obj)->value(0).toInt();*/\
                                                (flag) = ((obj)->value(0).toInt() == 21930)?1:0;}

// QSqlQuery::lastInsertId() почему-то не работает, поэтому так:
#define QUERY_LAST_INS_ID(obj,flag,id)      /*qDebug() << "(!!!) nDBErr=" << (flag) << ". Executing SELECT LAST_INSERT_ID() query...";*/ if ((flag)){\
                                                (flag) = (obj)->exec("SELECT LAST_INSERT_ID();");\
                                                if ((flag)){(obj)->first();\
                                                    (id) = (obj)->value(0).toInt();}}
#define QUERY_BEGIN                         QString("BEGIN;")
#define QUERY_COMMIT                        QString("COMMIT;")
#define QUERY_ROLLBACK                      QString("ROLLBACK;")
#define QUERY_INS_DEVICE_MODEL              QString("INSERT INTO `device_models` (`name`, `position`, `maker`, `device`) VALUES ('%1', %2, %3, %4);")
#define QUERY_INS_CLIENT                    QString("INSERT INTO `clients` (`creator`, `name`, `surname`, `patronymic`, `agent_phone_mask`, `agent2_phone_mask`, `address`, `post_index`, `passport_num`, `passport_date`, `passport_organ`, `state`, `type`, `birthday`, `memorial`, `notes`, `is_regular`, `is_dealer`, `balance_enable`, `prefer_cashless`, `take_long`, `ignore_calls`, `is_bad`, `is_realizator`, `is_agent`, `visit_source`, `photo_id`, `INN`, `KPP`, `OGRN`, `web_password`, `ur_name`, `email`, `icq`, `skype`, `viber`, `telegram`, `site`, `whatsapp`, `agent_name`, `agent_surname`, `agent_patronymic`, `agent_phone`, `agent_phone_clean`, `agent2_name`, `agent2_surname`, `agent2_patronymic`, `agent2_phone`, `agent2_phone_clean`, `created`, `balance`, `price_col`, `repairs`, `purchases`, `token`) VALUES\
                                                                            (%1, '%2', '%3', '%4', 1, 1, '%5', NULL, NULL, NULL, NULL, 1, %6, NULL, NULL, NULL, 0, 0, 0, 0, 0, 0, 0, 0, 0, %7, NULL, NULL, NULL, NULL, '%8', '', '%9', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, UTC_TIMESTAMP(), 0, 0, 1, 0, NULL);")
#define QUERY_INS_PHONE(P,p,M,C,T,t)        QString("INSERT INTO `tel` (\
                                                `phone`, `phone_clean`, `mask`, `customer`, `type`, `note`\
                                                ) VALUES (\
                                                '%1','%2',%3,%4,%5,'%6'\
                                                );").arg(P).arg(p).arg(M).arg(C).arg(T).arg(t)
#define QUERY_INS_USER_ACTIVITY(action)     QString("INSERT INTO `users_activity` (`user_id`, `datetime_`, `address`, `notes`, `app_version`, `machine_name`) VALUES (%1, UTC_TIMESTAMP(), SUBSTRING_INDEX(USER(), '@', -1), '%2', '%3', '%4');").arg(userData->value("id").toInt()).arg((action)).arg(APP_VER).arg("TODO:hostname")
#define QUERY_UPD_LAST_USER_ACTIVITY(id)    QString("UPDATE `users` SET `last_activity`=UTC_TIMESTAMP() WHERE `id` = %1;").arg((id))
#define QUERY_INS_WORKSHOP                  QString("INSERT INTO `workshop` (\
                                                `Hidden`, `Title`, `client`, `type`, `maker`, `model`, `serial_number`, `company`, `office`, `start_office`, `manager`, `current_manager`, `master`, `diagnostic_result`, `in_date`, `out_date`, `state`, `new_state`, `user_lock`, `lock_datetime`, `express_repair`, `quick_repair`, `is_warranty`, `is_repeat`, `payment_system`, `is_card_payment`, `can_format`, `print_check`, `box`, `warranty_label`, `ext_notes`, `is_prepaid`, `prepaid_type`, `prepaid_summ`, `prepaid_order`, `is_pre_agreed`, `is_debt`, `pre_agreed_amount`, `repair_cost`, `real_repair_cost`, `parts_cost`, `fault`, `complect`, `look`, `thirs_party_sc`, `last_save`, `last_status_changed`, `warranty_days`, `barcode`, `reject_reason`, `informed_status`, `image_ids`, `color`, `order_moving`, `early`, `ext_early`, `issued_msg`, `sms_inform`, `invoice`, `cartridge`, `vendor_id`, `termsControl`\
                                                ) VALUES(\
                                                0, '%1', %2, %3, %4, %5, '%6', %7, %8, %8, %9, %9, %10, '', UTC_TIMESTAMP(), NULL, 0, 0, NULL, NULL, %11, %12, %13, %14, %15, %16, %17, %18, %19, NULL, '%20', %21, %22, %23, NULL, %24, 0, %25, 0, 0, 0, '%26', '%27', '%28', %29, UTC_TIMESTAMP(), NULL, 0, '%30', '', 0, '', NULL, NULL, %31, %32, NULL, 1, NULL, NULL, NULL, 1\
                                                );")
#define QUERY_INS_REPAIR_COMMENT            QString("INSERT INTO `comments` (\
                                                `text`, `created`, `user`, `remont`, `client`, `task_id`, `part_request`\
                                                ) VALUES (\
                                                '%1', UTC_TIMESTAMP(), %2, %3, NULL, NULL, NULL\
                                                );")

// запись в журнал действий: Устройство принято в ремонт, Деталь добавлена в ремонт, Деталь продана в ремонте; UOCRI = user,office,client,repair,item, т. е. связанные ключи других таблиц
#define QUERY_INS_LOG(G,T,U,O,C,R,I,D,B,t)  QString("INSERT INTO `logs` (\
                                                `group`, `type`, `arh`, `user`, `created`, `values`, `values_after`, `office`, `client`, `repair`, `item`, `document`, `cash_order`, `part_request`, `notes`\
                                                ) VALUES (\
                                                %1, %2, 0, %3, UTC_TIMESTAMP(), NULL, NULL, %4, %5, %6, %7, %8, NULL, %9, '%10'\
                                                );").arg(G)\
                                                .arg(T)\
                                                .arg(U)\
                                                .arg(O)\
                                                .arg(C)\
                                                .arg(R)\
                                                .arg(I)\
                                                .arg(D)\
                                                .arg(B)\
                                                .arg(t)
#define QUERY_INS_CASH(T,S,s,C,U,O,t,R,A)   QString("INSERT INTO `cash_orders` (\
                                                `created`, `type`, `summa`, `summa_str`, `invoice`, `client`, `to_user`, `user`, `company`, `office`, `notes`, `repair`, `document`, `img`, `payment_system`, `card_fee`, `is_backdate`, `card_info`, `customer_email`, `fdn`, `payment_item_sign`\
                                                ) VALUES (\
                                                UTC_TIMESTAMP(), %1, %2, '%3', NULL, %4, NULL, %5, 1, %6, '%7', %8, NULL, NULL, %9, 0, 0, NULL, '', NULL, NULL\
                                                );").arg(T)\
                                                .arg(S)\
                                                .arg(s)\
                                                .arg(C)\
                                                .arg(U)\
                                                .arg(O)\
                                                .arg(t)\
                                                .arg(R)\
                                                .arg(A)
#define QUERY_VRFY_CASH(T,S,s,C,U,O,t,R,A)  QString("SELECT IF(`type` = %1 AND `summa` = %2 /*AND `summa_str` = %3*/ AND `client` = %4 AND `user` = %5 AND `office` = %6 /*AND `notes` = %7*/ AND `repair` = %8 AND `payment_system` = %9, 21930, 0)\
                                                    FROM `cash_orders`\
                                                    WHERE `id` = LAST_INSERT_ID();").arg(T)\
                                                .arg(S)\
                                                .arg(s)\
                                                .arg(C)\
                                                .arg(U)\
                                                .arg(O)\
                                                .arg(t)\
                                                .arg(R)\
                                                .arg(A)

#define QUERY_INS_FIELDS(T, R, I, t)  QString("INSERT INTO `field_values` (`field_id`, `repair_id`, `item_id`, `value`) VALUES (%1, %2, %3, '%4');").arg(T)\
                                                .arg(R)\
                                                .arg(I)\
                                                .arg(t)

#endif // COM_SQL_QUERIES_H

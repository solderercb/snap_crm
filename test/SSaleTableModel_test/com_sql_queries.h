/* Это минимальный набор для теста
*/
#ifndef COM_SQL_QUERIES_H
#define COM_SQL_QUERIES_H

#define QUERY_BEGIN                         QString("BEGIN;")
#define QUERY_COMMIT                        QString("COMMIT;")
#define QUERY_ROLLBACK                      QString("ROLLBACK;")
#define QUERY_COMMIT_ROLLBACK(obj,flag)     if (flag == 0)   /* в случае ошибки выполнения запроса нужно всё откатить */\
                                            {\
                                                (obj)->exec(QUERY_ROLLBACK);\
                                            }\
                                            else\
                                            {\
                                                (obj)->exec(QUERY_COMMIT);\
                                            }
#define QUERY_EXEC(obj,flag)                if ((flag)) (flag) = (obj)->exec
#define QUERY_EXEC_TH(obj,flag,query) if ((flag)) if(!(obj)->exec((query))) Global::throwError((obj)->lastError());
#define QUERY_EXEC_VRFY(obj,id)             (obj)->first();\
                                            if ((obj)->value(0).toInt() != 21930){\
                                                Global::throwError(Global::ThrowType::IntegrityError,\
                                                                    QObject::tr("Ошибка целостности данных баланса (id = %1)").arg((id)));\
                                            }

#define QUERY_NEW_ID(column, table)         QString("SELECT (MAX(`%1`)) + 1 AS 'id', COUNT(1) AS 'count' FROM `%2`;")\
                                                .arg((column), (table))

#define QUERY_LAST_INS_ID_TH(obj,flag,id)   if ((flag)){\
                                                if(!(obj)->exec("SELECT LAST_INSERT_ID();"))\
                                                    Global::throwError((obj)->lastError());\
                                                (obj)->first();\
                                                (id) = (obj)->value(0).toInt();}

#define QUERY_SEL_STORE_ITEMS_ITEM_PRICE(column)    QString("SELECT %1 FROM store_items WHERE `id` = :id;").arg(column)
#define QUERY_SEL_STORE_ITEM_UID_NAME(id)   QString("SELECT CONCAT(`articul`, '-', `id`) AS 'UID', `name` FROM store_items WHERE `id` = %1;").arg((id))

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

#define QUERY_SEL_REPAIR_SALES_QTY(id)       QString("SELECT `count` FROM store_int_reserve WHERE `id` = %1;").arg((id))

#define QUERY_SEL_ITEMS_IN_DOC(id)                      QString(\
                                                        "SELECT\n"\
                                                        "	t1.`id`,\n"\
                                                        "	CONCAT(LPAD(t2.articul, 6, '0'), '-', LPAD(t2.id, 6, '0')) AS 'UID',\n"\
                                                        "	t2.`name`,\n"\
                                                        "	t1.`count`,\n"\
                                                        "	t2.`count` - t2.`reserved` AS 'avail',\n"\
                                                        "	t1.`price`,\n"\
                                                        "	0 AS 'summ',\n"\
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
                                                        "  0 AS 'summ',\n"\
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
                                                        "  0 AS 'summ',\n"\
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

#define QUERY_SEL_WORK_FROM_PRICELIST(uid, price_field_name)  QString(\
                                                        "SELECT\n"\
                                                        "  0 AS 'id',\n"\
                                                        "  LPAD(`id`, 6, '0') AS 'UID',\n"\
                                                        "  `name`,\n"\
                                                        "  1 AS 'count',\n"\
                                                        "  9999 AS 'avail',\n"\
                                                        "  %2 AS 'price',\n"\
                                                        "  0 AS 'summ',\n"\
                                                        "  '' AS 'box',\n"\
                                                        "  '' AS 'sn',\n"\
                                                        "  `warranty`,\n"\
                                                        "  0 AS 'user',\n"\
                                                        "  0 AS 'is_realization',\n"\
                                                        "  `master_part` AS 'return_percent',\n"\
                                                        "  0 AS 'state',\n"\
                                                        "  NULL  AS 'notes',\n"\
                                                        "  `id` AS 'item_id',\n"\
                                                        "  0 AS 'in_price',\n"\
                                                        "  0 AS 'obj_id',\n"\
                                                        "  `vendor_id` AS 'dealer',\n"\
                                                        "  0 AS 'buyer',\n"\
                                                        "  NULL AS 'created',\n"\
                                                        "  NULL AS 'work_id',\n"\
                                                        "  0 AS 'is_item',\n"\
                                                        "  0 AS 'work_type'\n"\
                                                        "FROM\n"\
                                                        "  workshop_price\n"\
                                                        "WHERE\n"\
                                                        "  `id` IN (%1);")\
                                                        .arg((uid))\
                                                        .arg((price_field_name))

#define QUERY_SEL_PART_FROM_BASKET(uid, count)  QString(\
                                                        "SELECT\n"\
                                                        "  `id`,\n"\
                                                        "  CONCAT(LPAD(`articul`, 6, '0'), '-', LPAD(`item_id`, 6, '0')) AS 'UID',\n"\
                                                        "  `name`,\n"\
                                                        "  %2 AS 'count',\n"\
                                                        "  `avail`,\n"\
                                                        "  `price`,\n"\
                                                        "  0 AS 'summ',\n"\
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

#define QUERY_SEL_REPAIR_WORKS_AND_PARTS(id)    QString(\
                                                "SELECT `id`, `UID`, `name`, `count`, `avail`, `price`, `summ`, `box`, `sn`, `warranty`, `user`, `is_realization`, `return_percent`, `state`, `notes`, `item_id`, `in_price`, `obj_id`, `dealer`, `buyer`, `created`, `work_id`, `is_item`, `work_type` FROM (\n"\
                                                "SELECT\n"\
                                                "  t1.`id`,\n"\
                                                "  t1.`price_id` AS 'UID',\n"\
                                                "  t1.`name`,\n"\
                                                "  t1.`count`,\n"\
                                                "  999999 AS 'avail',\n"\
                                                "  ROUND(t1.`price`, 2) AS 'price',\n"\
                                                "  0 AS 'summ',\n"\
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

#endif // COM_SQL_QUERIES_H

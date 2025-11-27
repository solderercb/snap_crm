/* Это минимальный набор для теста
*/
#ifndef COM_SQL_QUERIES_H
#define COM_SQL_QUERIES_H

#define QUERY_EXEC(obj,flag)                /*qDebug() << "(!!!) nDBErr=" << (flag) << ". Executing query...";*/ if ((flag)) (flag) = (obj)->exec
#define QUERY_EXEC_TH(obj,flag,query) if ((flag)) if(!(obj)->exec((query))) Global::throwError((obj)->lastError());
#define QUERY_NEW_ID(column, table)         QString("SELECT (MAX(`%1`)) + 1 AS 'id', COUNT(1) AS 'count' FROM `%2`;")\
                                                .arg((column), (table))

#define QUERY_LAST_INS_ID_TH(obj,flag,id)   if ((flag)){\
                                                if(!(obj)->exec("SELECT LAST_INSERT_ID();"))\
                                                    Global::throwError((obj)->lastError());\
                                                (obj)->first();\
                                                (id) = (obj)->value(0).toInt();}

#define QUERY_SEL_ADDITIONAL_FIELDS_TYPES(isRepair, id) QString(\
                                                "SELECT\n"\
                                                "  `id`\n"\
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

#define QUERY_SEL_REPAIR_ADD_FIELDS(R)      QString("SELECT t1.`id`, t1.`field_id`, t1.`repair_id`, t1.`item_id`, t1.`value`, t2.`name`, t2.`printable` FROM `field_values` AS t1 LEFT JOIN `fields` AS t2 ON t1.`field_id` = t2.`id` WHERE t1.`repair_id` = %1 ORDER BY t1.`field_id` ASC;").arg((R))
#define QUERY_SEL_ITEM_ADD_FIELDS(I)        QString("SELECT t1.`id`, t1.`field_id`, t1.`repair_id`, t1.`item_id`, t1.`value`, t2.`name`, t2.`printable` FROM `field_values` AS t1 LEFT JOIN `fields` AS t2 ON t1.`field_id` = t2.`id` WHERE t1.`item_id` = %1 ORDER BY t1.`field_id` ASC;").arg((I))
#define QUERY_SEL_ADD_FIELD(id)             QString("SELECT t1.`id`, t1.`field_id`, t1.`repair_id`, t1.`item_id`, t1.`value`, t2.`name`, t2.`printable` FROM `field_values` AS t1 LEFT JOIN `fields` AS t2 ON t1.`field_id` = t2.`id` WHERE t1.`id` = %1;").arg((id))

#endif // COM_SQL_QUERIES_H

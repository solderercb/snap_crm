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

#define QUERY_SEL_INVOICE(id)               QString(\
                                                        "SELECT\r\n"\
                                                        "  t1.`id`,\r\n"\
                                                        "  t1.`num`,\r\n"\
                                                        "  t1.`created`,\r\n"\
                                                        "  t1.`user`,\r\n"\
                                                        "  t1.`seller`,\r\n"\
                                                        "  t1.`customer`,\r\n"\
                                                        "  t1.`paid`,\r\n"\
                                                        "  t1.`tax`,\r\n"\
                                                        "  t1.`summ`,\r\n"\
                                                        "  t1.`notes`,\r\n"\
                                                        "  t1.`total`,\r\n"\
                                                        "  t1.`state`,\r\n"\
                                                        "  t1.`office`,\r\n"\
                                                        "  t1.`type`,\r\n"\
                                                        "  t2.`ur_name` AS 'customer_name',\r\n"\
                                                        "  t2.`client` AS 'client_id'\r\n"\
                                                        "FROM\r\n"\
                                                        "  `invoice` AS t1\r\n"\
                                                        "LEFT JOIN\r\n"\
                                                        "  `banks` AS t2\r\n"\
                                                        "ON\r\n"\
                                                        "  t1.`customer` = t2.`id`\r\n"\
                                                        "WHERE\r\n"\
                                                        "  t1.`id` = %1;")\
                                                        .arg((id))

#endif // COM_SQL_QUERIES_H

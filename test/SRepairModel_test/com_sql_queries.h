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

#define QUERY_SEL_REPAIR_PAYMENTS(R, T)     QString("SELECT IFNULL(SUM(`summa`), 0) AS 'summa' FROM `cash_orders` WHERE `repair` = %1 AND `type` = IF(%2, %2, `type`);").arg((R)).arg((T))
#define QUERY_SEL_REPAIR_WORKS(R)           QString("SELECT SUM(`price` * `count`) AS 'summa' FROM `works` WHERE `repair` = %1;").arg((R))
#define QUERY_SEL_REPAIR_PARTS(R)           QString("SELECT SUM(`price` * `count`) AS `summa` FROM `store_int_reserve` WHERE `state` IN (2, 3) AND `repair_id` = %1;").arg((R))
#define QUERY_SEL_REPAIR_LOCK(db, id)       QString("SELECT IFNULL(t1.`user_lock`, 0) AS 'user_id', IF(t3.`USER` IS NULL, 0, 1) AS 'is_user_online', t1.`lock_datetime` FROM `%1`.`workshop` AS t1 LEFT JOIN `%1`.`users` AS t2 ON t1.`user_lock` = t2.`id` LEFT JOIN `information_schema`.`PROCESSLIST` AS t3 ON t2.`username` = t3.`USER` AND  t3.`DB` = '%1' WHERE t1.`id` = %2 GROUP BY t1.`id`;").arg((db)).arg((id))

#endif // COM_SQL_QUERIES_H

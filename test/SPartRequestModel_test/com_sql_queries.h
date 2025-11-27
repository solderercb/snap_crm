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

#define QUERY_SEL_PRT_RQST_MANAGERS(permId) QString("SELECT t1.`username`, t1.`id` FROM `users` AS t1 LEFT JOIN `roles_users` AS t2 ON t1.id = t2.user_id LEFT JOIN `permissions_roles` AS t3 ON t2.`role_id` = t3.`role_id` WHERE t3.`permission_id` = %1 GROUP BY t1.`id`;").arg((permId))

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

#endif // COM_SQL_QUERIES_H

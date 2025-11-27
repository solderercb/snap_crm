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

#define QUERY_SEL_COMMENTS(field, id)           QString("SELECT `id`, `created`, `user`, `text` FROM `comments` WHERE `%1` = %2 ORDER BY `created` DESC,`id` DESC;").arg((field)).arg((id))

#endif // COM_SQL_QUERIES_H

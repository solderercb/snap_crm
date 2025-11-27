/* Это минимальный набор для теста
*/
#ifndef COM_SQL_QUERIES_H
#define COM_SQL_QUERIES_H

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

#define QUERY_SEL_CLIENT_AD_TYPES           QString("SELECT `name`, `id` FROM visit_sources WHERE `enabled` ORDER BY `position` ASC;")

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

#define QUERY_SEL_CLIENT_PHONES(id)         QString("SELECT `id`, `phone`, `phone_clean`, `mask`, `customer`, `type`, `note`, `viber`, `telegram`, `whatsapp`, `notify` FROM `tel` WHERE `customer` = " + QString::number((id)) + " ORDER BY `type` DESC, `id` DESC")

#endif // COM_SQL_QUERIES_H

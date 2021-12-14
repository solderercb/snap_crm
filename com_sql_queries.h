#ifndef COM_SQL_QUERIES_H
#define COM_SQL_QUERIES_H

#define QUERY_CLIENT_AD_TYPES   "SELECT '' AS 'name', '' AS 'id' UNION ALL (SELECT `name`, `id` FROM visit_sources WHERE `enabled` ORDER BY `position` ASC);"
#define QUERY_CLIENT_PHONES(id) "SELECT `phone`, `mask` FROM `tel` WHERE `customer` = " + QString::number((id)) + " ORDER BY `type` DESC, `id` DESC LIMIT 2"
#endif // COM_SQL_QUERIES_H

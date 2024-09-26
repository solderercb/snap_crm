UPDATE
  `salary` AS t1
  LEFT JOIN `balance` AS t2
    ON t1.`balance_record` = t2.`id`
  LEFT JOIN `cash_orders` AS t3
    ON t1.`payment_date` = t3.`created`
    AND IF(t2.`summ` < 0, t1.`summ`- t2.`summ`, t1.`summ`) = -t3.`summa`
  LEFT JOIN `schemaversions`
    ON `schemaversions`.`scriptname` LIKE '%script000022.sql'
SET
  t1.`summ` = IF(t3.`summa` IS NOT NULL, -t3.`summa`, IF(t2.`summ` IS NULL, t1.`summ`, 0))
WHERE
    `schemaversions`.`scriptname` IS NULL;

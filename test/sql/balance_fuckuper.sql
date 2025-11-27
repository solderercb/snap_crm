# Таблица для хранения "глобальных" переменных пользователя.
CREATE TABLE IF NOT EXISTS `global_user_variables` (
  `name` CHAR(50) NULL DEFAULT NULL,
  `value` CHAR(50) NULL DEFAULT NULL,
  UNIQUE INDEX `name` (`name`)
)
COLLATE='utf8_general_ci'
ENGINE=InnoDB;

INSERT INTO `global_user_variables` (`name`, `value`) VALUES ('test_trig_balance_fuckuper', 0) ON DUPLICATE KEY UPDATE `value` = `value`;

DELIMITER $$

DROP TRIGGER IF EXISTS `balance_fuckuper`$$
CREATE DEFINER=`root`@`%` TRIGGER `balance_fuckuper` BEFORE INSERT ON `balance` FOR EACH ROW
BEGIN
  SELECT IFNULL(t1.`value`, 0) FROM (SELECT 0 AS 'value') `dummy` LEFT JOIN (SELECT `value` FROM `global_user_variables` WHERE `name` = 'test_trig_balance_fuckuper') `t1` ON 1 INTO @is_trig_en;

  IF(@is_trig_en) THEN
    SET
      NEW.`summ` = NEW.`summ` + 1;
  END IF;
END$$

DELIMITER ;

SELECT * FROM `global_user_variables` WHERE `name` LIKE 'test_trig%';
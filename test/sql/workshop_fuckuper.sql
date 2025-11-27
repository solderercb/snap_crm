# Таблица для хранения "глобальных" переменных пользователя.
CREATE TABLE IF NOT EXISTS `global_user_variables` (
  `name` CHAR(50) NULL DEFAULT NULL,
  `value` CHAR(50) NULL DEFAULT NULL,
  UNIQUE INDEX `name` (`name`)
)
COLLATE='utf8_general_ci'
ENGINE=InnoDB;

INSERT INTO `global_user_variables` (`name`, `value`) VALUES ('test_trig_repair_status_fuckuper', 1) ON DUPLICATE KEY UPDATE `value` = `value`;
INSERT INTO `global_user_variables` (`name`, `value`) VALUES ('test_trig_repair_box_fuckuper', 1) ON DUPLICATE KEY UPDATE `value` = `value`;
INSERT INTO `global_user_variables` (`name`, `value`) VALUES ('test_trig_repair_informed_status_fuckuper', 1) ON DUPLICATE KEY UPDATE `value` = `value`;

DELIMITER $$

DROP TRIGGER IF EXISTS `test_workshop_fuckuper`$$
CREATE DEFINER=`root`@`%` TRIGGER `test_workshop_fuckuper` BEFORE UPDATE ON `workshop` FOR EACH ROW
BEGIN
  SELECT IFNULL(t1.`value`, 0) FROM (SELECT 0 AS 'value') `dummy` LEFT JOIN (SELECT `value` FROM `global_user_variables` WHERE `name` = 'test_trig_repair_status_fuckuper') `t1` ON 1 INTO @is_trig_status_en;
  SELECT IFNULL(t1.`value`, 0) FROM (SELECT 0 AS 'value') `dummy` LEFT JOIN (SELECT `value` FROM `global_user_variables` WHERE `name` = 'test_trig_repair_box_fuckuper') `t1` ON 1 INTO @is_trig_box_en;
  SELECT IFNULL(t1.`value`, 0) FROM (SELECT 0 AS 'value') `dummy` LEFT JOIN (SELECT `value` FROM `global_user_variables` WHERE `name` = 'test_trig_repair_informed_status_fuckuper') `t1` ON 1 INTO @is_trig_informed_status_en;

  IF(@is_trig_status_en AND NEW.`state` <> OLD.`state`) THEN
      SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = 'test_workshop_fuckuper (1): exception was forced for testing purposes';
  END IF;
  IF(@is_trig_box_en AND IFNULL(NEW.`box`,'NULL') <> IFNULL(OLD.`box`,'NULL')) THEN
      SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = 'test_workshop_fuckuper (2): exception was forced for testing purposes';
  END IF;
  IF(@is_trig_informed_status_en AND NEW.`informed_status` <> OLD.`informed_status`) THEN
      SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = 'test_workshop_fuckuper (3): exception was forced for testing purposes';
  END IF;
END$$

DELIMITER ;

SELECT * FROM `global_user_variables` WHERE `name` LIKE 'test_trig%';
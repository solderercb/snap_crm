ALTER TABLE `payment_systems`
    ADD COLUMN `office` INT(11) NULL DEFAULT NULL AFTER `is_enable`,
    ADD COLUMN `is_show_balance` TINYINT(1) NOT NULL DEFAULT '0' AFTER `office`,
    ADD CONSTRAINT `FK_payment_systems_offices` FOREIGN KEY (`office`) REFERENCES `offices` (`id`) ON UPDATE NO ACTION ON DELETE NO ACTION;

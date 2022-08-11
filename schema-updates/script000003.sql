CREATE TABLE `tel_types` (
	`id` INT(11) NOT NULL AUTO_INCREMENT,
	`name` TINYTEXT NOT NULL,
	`mask` TINYTEXT NOT NULL,
	`order` INT(11) UNSIGNED NOT NULL,
	`enabled` BIT(1) NOT NULL DEFAULT b'1',
	PRIMARY KEY (`id`)
)
COLLATE='utf8mb4_general_ci'
AUTO_INCREMENT=1;

INSERT INTO `tel_types` (`name`, `mask`, `order`) VALUES ('мобильный (UA)', '(+38) 999 999-99-99', 2);
INSERT INTO `tel_types` (`name`, `mask`, `order`) VALUES ('городской', '99-99-99', 3);
INSERT INTO `tel_types` (`name`, `mask`, `order`) VALUES ('мобильный (RU)', '+7 999 999-99-99', 1);

ALTER TABLE `tel`
	CHANGE COLUMN `mask` `mask` INT(11) UNSIGNED NOT NULL AFTER `phone_clean`,
	ADD COLUMN `viber` BIT NOT NULL DEFAULT b'0' AFTER `note`,
	ADD COLUMN `telegram` BIT NOT NULL DEFAULT b'0' AFTER `viber`,
	ADD COLUMN `whatsapp` BIT NOT NULL DEFAULT b'0' AFTER `telegram`;

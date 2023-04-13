CREATE TABLE `doc_templates_snap` (
	`id` INT(11) NOT NULL AUTO_INCREMENT,
	`name` VARCHAR(50) NOT NULL,
	`description` VARCHAR(254) NULL DEFAULT NULL,
	`data` MEDIUMBLOB NOT NULL,
	`checksum` BINARY(20) NOT NULL,
	PRIMARY KEY (`id`)
)
COLLATE='utf8_general_ci'
ENGINE=InnoDB
AUTO_INCREMENT=0;

INSERT INTO `doc_templates_snap` (`name`,  `description`,  `data`,  `checksum`)
	SELECT  `name`,  `description`,  `data`,  `checksum` FROM `doc_templates` WHERE `app` = 'snap';

DELETE FROM `doc_templates` WHERE `app` = 'snap';

ALTER TABLE `doc_templates`
	DROP COLUMN `app`;


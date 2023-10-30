ALTER TABLE `clients`
	ADD COLUMN `short_name` VARCHAR(40) NULL DEFAULT NULL AFTER `employee`;

INSERT INTO `ascapp_test`.`user_params` (`name`, `default_value`) VALUES ('alternate_rows_background', '0');


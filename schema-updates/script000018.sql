ALTER TABLE `clients`
	ADD COLUMN `short_name` VARCHAR(40) NULL DEFAULT NULL AFTER `employee`;

INSERT INTO `user_params` (`id`, `name`, `default_value`) VALUES (4, 'alternate_rows_background', '0');


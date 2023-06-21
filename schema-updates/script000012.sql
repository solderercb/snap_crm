CREATE TABLE `user_params` (
	`id` INT(11) NOT NULL AUTO_INCREMENT,
	`name` TINYTEXT NOT NULL,
	`default_value` TEXT NULL,
	PRIMARY KEY (`id`)
)
COLLATE='utf8mb4_general_ci'
ENGINE=InnoDB;

CREATE TABLE `users_params` (
	`id` INT(11) NOT NULL AUTO_INCREMENT,
	`user_id` INT(11) NOT NULL,
	`param_id` INT(11) NOT NULL,
	`value` TEXT NULL,
	PRIMARY KEY (`id`),
	UNIQUE INDEX `unique_user_param` (`user_id`, `param_id`),
	INDEX `FK_params` (`param_id`),
	CONSTRAINT `FK_params` FOREIGN KEY (`param_id`) REFERENCES `user_params` (`id`),
	CONSTRAINT `FK_users` FOREIGN KEY (`user_id`) REFERENCES `users` (`id`)
)
COLLATE='utf8mb4_general_ci'
ENGINE=InnoDB;

INSERT INTO `user_params` (`id`, `name`, `default_value`) VALUES (1, 'autosave_part_list', '1');

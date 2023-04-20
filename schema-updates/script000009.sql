CREATE TABLE `salary_repairs` (
	`id` INT(11) UNSIGNED NOT NULL AUTO_INCREMENT,
	`repair` INT(11) NULL DEFAULT NULL,
	`cartridge` INT(11) NULL DEFAULT NULL,
	`user` INT(11) NOT NULL,
	`summ` DECIMAL(11,5) NULL DEFAULT NULL,
	PRIMARY KEY (`id`),
	INDEX `FK_salary_repairs_c_workshop` (`cartridge`),
	INDEX `FK_salary_repairs_users` (`user`),
	INDEX `FK_salary_repairs_workshop` (`repair`),
	CONSTRAINT `FK_salary_repairs_c_workshop` FOREIGN KEY (`cartridge`) REFERENCES `c_workshop` (`id`),
	CONSTRAINT `FK_salary_repairs_users` FOREIGN KEY (`user`) REFERENCES `users` (`id`),
	CONSTRAINT `FK_salary_repairs_workshop` FOREIGN KEY (`repair`) REFERENCES `workshop` (`id`)
)
COMMENT='Список ремонтов, за которые была выплачена заработная плата'
COLLATE='utf8mb4_general_ci'
ENGINE=InnoDB;

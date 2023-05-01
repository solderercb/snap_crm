CREATE TABLE `salary_repairs` (
	`id` INT(11) UNSIGNED NOT NULL AUTO_INCREMENT,
	`repair` INT(11) NULL DEFAULT NULL COMMENT 'выплата за ремонт',
	`cartridge` INT(11) NULL DEFAULT NULL COMMENT 'выплата за картридж',
	`user` INT(11) NOT NULL COMMENT 'id сотрудника, которому была сделана эта выплата',
	`summ_repair` DECIMAL(11,5) NULL DEFAULT NULL COMMENT 'сумма, выплаченная сотруднику за работы и детали',
	`summ_recept` DECIMAL(11,5) NULL DEFAULT NULL COMMENT 'сумма, выплаченная сотруднику за приём уст-ва в ремонт',
	`summ_issue` DECIMAL(11,5) NULL DEFAULT NULL COMMENT 'сумма, выплаченная сотруднику за выдачу уст-ва',
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

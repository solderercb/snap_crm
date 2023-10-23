ALTER TABLE `materials`
	ADD COLUMN `salary_summ` DECIMAL(19,4) NULL DEFAULT NULL AFTER `works_price`;
ALTER TABLE `works`
	ADD COLUMN `salary_summ` DECIMAL(19,4) NULL DEFAULT NULL COMMENT 'сумма выплаты сотруднику' AFTER `pay_repair_quick`;

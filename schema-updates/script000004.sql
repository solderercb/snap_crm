ALTER TABLE `fields`
	ADD COLUMN `dev_match` TINYINT(1) NOT NULL DEFAULT '0' COMMENT '1 - производить поиск совпадений при приёме в ремонт' AFTER `archive`,
	ADD COLUMN `input_mask` TINYTEXT NULL AFTER `dev_match`;

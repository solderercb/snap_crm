ALTER TABLE `config`
	ADD COLUMN `version_snap` VARCHAR(50) NOT NULL AFTER `version`;
UPDATE `config` SET `version_snap`='0.0.0.96' WHERE `id`=1;
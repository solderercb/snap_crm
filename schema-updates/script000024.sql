ALTER TABLE `tech_reports`
    CHANGE COLUMN `device` `device` VARCHAR(1500) NOT NULL COLLATE 'utf8mb4_general_ci' AFTER `client`;

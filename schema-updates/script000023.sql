CREATE TABLE IF NOT EXISTS `parts_request_suppliers` (
	`id` INT(11) NOT NULL AUTO_INCREMENT,
	`request_id` INT(11) NOT NULL,
	`supplier_id` INT(11) NULL DEFAULT NULL,
	`supplier_url` TEXT NULL,
	`item_url` TEXT NOT NULL,
	`moq` INT(11) UNSIGNED NOT NULL DEFAULT '1' COMMENT 'Minimum order quantity',
	`price` DECIMAL(11,4) UNSIGNED NOT NULL DEFAULT '0.0000' COMMENT 'Price per piece',
	`select` TINYINT(3) UNSIGNED NOT NULL DEFAULT '0' COMMENT 'Final choice sign',
	`notes` VARCHAR(254) NULL DEFAULT NULL,
	PRIMARY KEY (`id`),
	INDEX `FK_parts_requests_suppliers_clients` (`supplier_id`),
	INDEX `FK_parts_requests_suppliers_parts_request` (`request_id`),
	CONSTRAINT `FK_parts_requests_suppliers_clients` FOREIGN KEY (`supplier_id`) REFERENCES `clients` (`id`) ON UPDATE NO ACTION ON DELETE NO ACTION,
	CONSTRAINT `FK_parts_requests_suppliers_parts_request` FOREIGN KEY (`request_id`) REFERENCES `parts_request` (`id`) ON UPDATE NO ACTION ON DELETE NO ACTION
)
COLLATE='utf8mb4_general_ci'
ENGINE=InnoDB
AUTO_INCREMENT=1;

INSERT INTO `ascapp_test`.`permissions` (`id`, `name`) VALUES ('109', 'viewAllPartsRequests');
INSERT INTO `permissions_roles` (`permission_id`, `role_id`) VALUES ('109', '1');
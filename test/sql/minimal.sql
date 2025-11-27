-- MySQL dump 10.13  Distrib 5.7.38, for Win64 (x86_64)
--
-- Host: 127.0.0.1    Database: snap_app_test
-- ------------------------------------------------------
-- Server version	5.7.38

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Table structure for table `additional_payments`
--

DROP TABLE IF EXISTS `additional_payments`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `additional_payments` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(254) NOT NULL,
  `payment_date` datetime NOT NULL,
  `user` int(11) NOT NULL,
  `to_user` int(11) NOT NULL,
  `price` decimal(19,4) NOT NULL,
  PRIMARY KEY (`id`),
  KEY `fk_additional_payments_users1_idx` (`user`),
  KEY `fk_additional_payments_users2_idx` (`to_user`),
  CONSTRAINT `fk_additional_payments_users1` FOREIGN KEY (`user`) REFERENCES `users` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT `fk_additional_payments_users2` FOREIGN KEY (`to_user`) REFERENCES `users` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `additional_payments`
--

LOCK TABLES `additional_payments` WRITE;
/*!40000 ALTER TABLE `additional_payments` DISABLE KEYS */;
/*!40000 ALTER TABLE `additional_payments` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `alembic_version`
--

DROP TABLE IF EXISTS `alembic_version`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `alembic_version` (
  `version_num` varchar(32) NOT NULL,
  PRIMARY KEY (`version_num`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `alembic_version`
--

LOCK TABLES `alembic_version` WRITE;
/*!40000 ALTER TABLE `alembic_version` DISABLE KEYS */;
/*!40000 ALTER TABLE `alembic_version` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `api_failed_logins`
--

DROP TABLE IF EXISTS `api_failed_logins`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `api_failed_logins` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `ip_address` int(11) unsigned DEFAULT NULL,
  `attempted_at` datetime NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `api_failed_logins`
--

LOCK TABLES `api_failed_logins` WRITE;
/*!40000 ALTER TABLE `api_failed_logins` DISABLE KEYS */;
/*!40000 ALTER TABLE `api_failed_logins` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `api_status_checks`
--

DROP TABLE IF EXISTS `api_status_checks`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `api_status_checks` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `repair` int(10) unsigned NOT NULL,
  `ip_address` int(10) unsigned DEFAULT NULL,
  `user_agent` text,
  `created` datetime NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `api_status_checks`
--

LOCK TABLES `api_status_checks` WRITE;
/*!40000 ALTER TABLE `api_status_checks` DISABLE KEYS */;
/*!40000 ALTER TABLE `api_status_checks` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `balance`
--

DROP TABLE IF EXISTS `balance`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `balance` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `client` int(11) NOT NULL,
  `summ` decimal(19,4) NOT NULL,
  `direction` tinyint(1) NOT NULL COMMENT '0 списание 1 зачисление',
  `reason` text COMMENT 'Основание',
  `created` datetime NOT NULL,
  `office` int(11) NOT NULL,
  `uid` int(11) NOT NULL,
  `dealer_payment` int(11) DEFAULT NULL,
  PRIMARY KEY (`id`),
  KEY `FK_balance_clients` (`client`),
  KEY `FK_balance_offices` (`office`),
  KEY `FK_balance_users` (`uid`),
  KEY `FK_balance_dealer_payments` (`dealer_payment`),
  CONSTRAINT `FK_balance_clients` FOREIGN KEY (`client`) REFERENCES `clients` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT `FK_balance_dealer_payments` FOREIGN KEY (`dealer_payment`) REFERENCES `dealer_payments` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT `FK_balance_offices` FOREIGN KEY (`office`) REFERENCES `offices` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT `FK_balance_users` FOREIGN KEY (`uid`) REFERENCES `users` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION
) ENGINE=InnoDB AUTO_INCREMENT=29 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `balance`
--

LOCK TABLES `balance` WRITE;
/*!40000 ALTER TABLE `balance` DISABLE KEYS */;
INSERT INTO `balance` VALUES (1,1952,1000.0000,1,'init','2000-01-01 09:00:00',1,1,NULL);
/*!40000 ALTER TABLE `balance` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `banks`
--

DROP TABLE IF EXISTS `banks`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `banks` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `ur_name` varchar(254) DEFAULT NULL COMMENT 'покупатель',
  `chief` varchar(254) DEFAULT NULL COMMENT 'руководитель',
  `accountant` varchar(254) DEFAULT NULL COMMENT 'бухгалтер',
  `bank` varchar(254) DEFAULT NULL COMMENT 'банк',
  `correspondent_account` varchar(254) DEFAULT NULL COMMENT 'кор. счёт',
  `inn` varchar(254) DEFAULT NULL COMMENT 'инн',
  `kpp` varchar(254) DEFAULT NULL COMMENT 'кпп',
  `ogrn` varchar(254) DEFAULT NULL COMMENT 'ОГРН',
  `address` varchar(254) DEFAULT NULL COMMENT 'адрес',
  `checking_account` varchar(254) DEFAULT NULL COMMENT 'расчётный счёт',
  `BIC` varchar(254) DEFAULT NULL COMMENT 'бик',
  `SWIFT` varchar(254) DEFAULT NULL,
  `IBAN` varchar(254) DEFAULT NULL,
  `type` int(11) DEFAULT NULL COMMENT '0 банковские реквизиты организации (ACP)',
  `client` int(11) DEFAULT NULL,
  `company` int(11) DEFAULT NULL,
  `email` varchar(254) DEFAULT NULL,
  `seal` int(11) DEFAULT NULL COMMENT 'печать организации',
  `chief_signature` int(11) DEFAULT NULL COMMENT 'подпись руководителя',
  `accountant_signature` int(11) DEFAULT NULL COMMENT 'подпись гл. бухгалтера',
  `archive` tinyint(1) NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`),
  KEY `fk_banks_clients1_idx` (`client`),
  KEY `fk_banks_companies1_idx` (`company`),
  KEY `ur_name` (`ur_name`),
  KEY `FK_banks_seal_img` (`seal`),
  KEY `FK_banks_chief_sig_img` (`chief_signature`),
  KEY `FK_banks_acc_sig_img` (`accountant_signature`),
  CONSTRAINT `FK_banks_acc_sig_img` FOREIGN KEY (`accountant_signature`) REFERENCES `images` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT `FK_banks_chief_sig_img` FOREIGN KEY (`chief_signature`) REFERENCES `images` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT `FK_banks_seal_img` FOREIGN KEY (`seal`) REFERENCES `images` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT `fk_banks_clients1` FOREIGN KEY (`client`) REFERENCES `clients` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT `fk_banks_companies1` FOREIGN KEY (`company`) REFERENCES `companies` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION
) ENGINE=InnoDB AUTO_INCREMENT=3 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `banks`
--

LOCK TABLES `banks` WRITE;
/*!40000 ALTER TABLE `banks` DISABLE KEYS */;
INSERT INTO `banks` VALUES (1,'Сервисный центр',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,1,NULL,NULL,NULL,NULL,0),(2,'\"Центр обслуживания управления образования и молодежи и его подведомственных учреждений\" Муниципального образования Республики',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,111,NULL,NULL,NULL,NULL,NULL,0);
/*!40000 ALTER TABLE `banks` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `boxes`
--

DROP TABLE IF EXISTS `boxes`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `boxes` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(45) NOT NULL,
  `places` int(11) NOT NULL DEFAULT '0' COMMENT 'Всего мест в коробке',
  `store_id` int(11) DEFAULT NULL COMMENT 'ID склада',
  `non_items` tinyint(1) NOT NULL DEFAULT '0',
  `color` char(9) DEFAULT NULL,
  PRIMARY KEY (`id`),
  KEY `fk_boxes_stores1_idx` (`store_id`),
  KEY `name` (`name`),
  KEY `places` (`places`),
  CONSTRAINT `fk_boxes_stores1` FOREIGN KEY (`store_id`) REFERENCES `stores` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION
) ENGINE=InnoDB AUTO_INCREMENT=3 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `boxes`
--

LOCK TABLES `boxes` WRITE;
/*!40000 ALTER TABLE `boxes` DISABLE KEYS */;
INSERT INTO `boxes` VALUES (1,'box1',1,NULL,1,NULL),(2,'box2',1,NULL,1,NULL);
/*!40000 ALTER TABLE `boxes` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `buyout`
--

DROP TABLE IF EXISTS `buyout`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `buyout` (
  `int` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `created_at` datetime NOT NULL,
  `document_id` int(11) NOT NULL,
  `name` varchar(50) DEFAULT NULL,
  `number` varchar(50) DEFAULT NULL,
  `authority` varchar(254) DEFAULT NULL,
  `customer_id` int(11) NOT NULL,
  `device_id` int(11) NOT NULL,
  `device_maker_id` int(11) NOT NULL,
  `device_model_id` int(11) NOT NULL,
  PRIMARY KEY (`int`),
  KEY `FK_buyout_docs` (`document_id`),
  KEY `FK_buyout_clients` (`customer_id`),
  KEY `FK_buyout_devices` (`device_id`),
  KEY `FK_buyout_device_makers` (`device_maker_id`),
  KEY `FK_buyout_device_models` (`device_model_id`),
  CONSTRAINT `FK_buyout_clients` FOREIGN KEY (`customer_id`) REFERENCES `clients` (`id`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `FK_buyout_device_makers` FOREIGN KEY (`device_maker_id`) REFERENCES `device_makers` (`id`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `FK_buyout_device_models` FOREIGN KEY (`device_model_id`) REFERENCES `device_models` (`id`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `FK_buyout_devices` FOREIGN KEY (`device_id`) REFERENCES `devices` (`id`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `FK_buyout_docs` FOREIGN KEY (`document_id`) REFERENCES `docs` (`id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `buyout`
--

LOCK TABLES `buyout` WRITE;
/*!40000 ALTER TABLE `buyout` DISABLE KEYS */;
/*!40000 ALTER TABLE `buyout` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `c_workshop`
--

DROP TABLE IF EXISTS `c_workshop`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `c_workshop` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `refill` tinyint(1) NOT NULL DEFAULT '0',
  `chip` tinyint(1) NOT NULL DEFAULT '0',
  `opc_drum` tinyint(1) NOT NULL DEFAULT '0',
  `c_blade` tinyint(1) NOT NULL DEFAULT '0',
  `card_id` int(11) NOT NULL,
  PRIMARY KEY (`id`),
  KEY `FK_c_workshop_cartridge_cards` (`card_id`),
  CONSTRAINT `FK_c_workshop_cartridge_cards` FOREIGN KEY (`card_id`) REFERENCES `cartridge_cards` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `c_workshop`
--

LOCK TABLES `c_workshop` WRITE;
/*!40000 ALTER TABLE `c_workshop` DISABLE KEYS */;
INSERT INTO `c_workshop` VALUES (1,1,0,0,0,1);
/*!40000 ALTER TABLE `c_workshop` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `cartridge_cards`
--

DROP TABLE IF EXISTS `cartridge_cards`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `cartridge_cards` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(254) NOT NULL,
  `maker` int(11) NOT NULL,
  `full_weight` double NOT NULL DEFAULT '0' COMMENT 'вес полного картриджа',
  `toner_weight` double NOT NULL DEFAULT '0' COMMENT 'вес заправки',
  `resource` int(11) NOT NULL DEFAULT '0' COMMENT 'ресурс страниц',
  `created` datetime NOT NULL,
  `user` int(11) NOT NULL,
  `notes` text,
  `photo` int(11) DEFAULT NULL,
  `color` int(11) NOT NULL DEFAULT '0',
  `archive` tinyint(1) NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`),
  KEY `FK_cartridge_cards_device_makers` (`maker`),
  KEY `FK_cartridge_cards_images` (`photo`),
  KEY `FK_cartridge_cards_users` (`user`),
  CONSTRAINT `FK_cartridge_cards_device_makers` FOREIGN KEY (`maker`) REFERENCES `device_makers` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT `FK_cartridge_cards_images` FOREIGN KEY (`photo`) REFERENCES `images` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT `FK_cartridge_cards_users` FOREIGN KEY (`user`) REFERENCES `users` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `cartridge_cards`
--

LOCK TABLES `cartridge_cards` WRITE;
/*!40000 ALTER TABLE `cartridge_cards` DISABLE KEYS */;
INSERT INTO `cartridge_cards` VALUES (1,'Xerox',42,0,0,0,'2000-01-01 09:00:00',1,NULL,NULL,0,0);
/*!40000 ALTER TABLE `cartridge_cards` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `cash_orders`
--

DROP TABLE IF EXISTS `cash_orders`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `cash_orders` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `created` datetime NOT NULL,
  `type` int(11) NOT NULL COMMENT '1 РКО Расходно-кассовый ордер без привязки\r\n2 РКО Оплата приходной накладной\r\n3 РКО Z отчёт (Выемка средств с кассы)\r\n7 Выемка/внесение средств\r\n11 ПКО без привязки\r\n13 Поступление денег на баланс клиента\r\n18 перемещение средств между счетами',
  `summa` decimal(19,4) NOT NULL,
  `summa_str` varchar(254) DEFAULT NULL,
  `invoice` int(11) DEFAULT NULL COMMENT '№ счёта',
  `client` int(11) DEFAULT NULL,
  `to_user` int(11) DEFAULT NULL,
  `user` int(11) NOT NULL,
  `company` int(11) NOT NULL,
  `office` int(11) NOT NULL,
  `notes` text,
  `repair` int(11) DEFAULT NULL,
  `document` int(11) DEFAULT NULL,
  `img` int(11) DEFAULT NULL,
  `payment_system` int(11) NOT NULL DEFAULT '0',
  `card_fee` decimal(19,4) NOT NULL DEFAULT '0.0000',
  `is_backdate` tinyint(1) NOT NULL DEFAULT '0',
  `card_info` int(11) DEFAULT NULL,
  `customer_email` varchar(254) DEFAULT NULL COMMENT 'телефон или e-mail',
  `fdn` int(10) unsigned DEFAULT NULL,
  `payment_item_sign` int(10) DEFAULT NULL COMMENT 'Признак предмета расчета',
  PRIMARY KEY (`id`),
  KEY `fk_cache_orders_users1_idx` (`user`),
  KEY `fk_cache_orders_clients1_idx` (`client`),
  KEY `fk_cache_orders_offices1_idx` (`office`),
  KEY `fk_cache_orders_workshop1_idx` (`repair`),
  KEY `fk_cash_orders_images1_idx` (`img`),
  KEY `fk_cash_orders_docs1_idx` (`document`),
  KEY `fk_cash_orders_companies1_idx` (`company`),
  KEY `fk_cash_orders_users1_idx` (`to_user`),
  KEY `created` (`created`),
  KEY `type` (`type`),
  KEY `FK_cash_orders_invoice` (`invoice`),
  KEY `FK_cash_orders_payment_systems` (`payment_system`),
  KEY `FK_cash_orders_pinpad_logs` (`card_info`),
  CONSTRAINT `FK_cash_orders_invoice` FOREIGN KEY (`invoice`) REFERENCES `invoice` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT `FK_cash_orders_payment_systems` FOREIGN KEY (`payment_system`) REFERENCES `payment_systems` (`system_id`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT `FK_cash_orders_pinpad_logs` FOREIGN KEY (`card_info`) REFERENCES `pinpad_logs` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT `fk_cache_orders_clients1` FOREIGN KEY (`client`) REFERENCES `clients` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT `fk_cache_orders_offices1` FOREIGN KEY (`office`) REFERENCES `offices` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT `fk_cache_orders_users1` FOREIGN KEY (`user`) REFERENCES `users` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT `fk_cache_orders_workshop1` FOREIGN KEY (`repair`) REFERENCES `workshop` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT `fk_cash_orders_companies1` FOREIGN KEY (`company`) REFERENCES `companies` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT `fk_cash_orders_docs1` FOREIGN KEY (`document`) REFERENCES `docs` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT `fk_cash_orders_images1` FOREIGN KEY (`img`) REFERENCES `images` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT `fk_cash_orders_users1` FOREIGN KEY (`to_user`) REFERENCES `users` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION
) ENGINE=InnoDB AUTO_INCREMENT=21 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `cash_orders`
--

LOCK TABLES `cash_orders` WRITE;
/*!40000 ALTER TABLE `cash_orders` DISABLE KEYS */;
INSERT INTO `cash_orders` VALUES (1,'2000-01-01 09:00:00',13,1000.0000,NULL,NULL,1952,NULL,1,1,1,NULL,NULL,NULL,NULL,0,0.0000,0,NULL,NULL,NULL,NULL);
/*!40000 ALTER TABLE `cash_orders` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `cdr`
--

DROP TABLE IF EXISTS `cdr`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `cdr` (
  `calldate` datetime NOT NULL,
  `clid` varchar(80) NOT NULL DEFAULT '',
  `src` varchar(254) NOT NULL DEFAULT '',
  `dst` varchar(80) NOT NULL DEFAULT '',
  `dcontext` varchar(80) NOT NULL DEFAULT '',
  `channel` varchar(80) NOT NULL DEFAULT '',
  `dstchannel` varchar(80) NOT NULL DEFAULT '',
  `lastapp` varchar(80) NOT NULL DEFAULT '',
  `lastdata` varchar(80) NOT NULL DEFAULT '',
  `duration` int(11) NOT NULL DEFAULT '0',
  `billsec` int(11) NOT NULL DEFAULT '0',
  `disposition` varchar(45) NOT NULL DEFAULT '',
  `amaflags` int(11) NOT NULL DEFAULT '0',
  `accountcode` varchar(20) NOT NULL DEFAULT '',
  `uniqueid` varchar(32) NOT NULL DEFAULT '',
  `userfield` varchar(255) NOT NULL DEFAULT '',
  KEY `calldate` (`calldate`),
  KEY `dst` (`dst`),
  KEY `accountcode` (`accountcode`),
  KEY `dcontext` (`dcontext`),
  KEY `disposition` (`disposition`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `cdr`
--

LOCK TABLES `cdr` WRITE;
/*!40000 ALTER TABLE `cdr` DISABLE KEYS */;
/*!40000 ALTER TABLE `cdr` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `clients`
--

DROP TABLE IF EXISTS `clients`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `clients` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `creator` int(11) NOT NULL DEFAULT '0',
  `name` varchar(254) NOT NULL,
  `surname` varchar(254) DEFAULT NULL,
  `patronymic` varchar(254) DEFAULT NULL,
  `agent_phone_mask` int(11) NOT NULL DEFAULT '1',
  `agent2_phone_mask` int(11) NOT NULL DEFAULT '1',
  `address` varchar(254) DEFAULT NULL,
  `post_index` varchar(254) DEFAULT NULL,
  `passport_num` varchar(254) DEFAULT NULL,
  `passport_date` date DEFAULT NULL,
  `passport_organ` varchar(254) DEFAULT NULL,
  `state` int(11) NOT NULL DEFAULT '1' COMMENT '0 архивный / 1 активный',
  `type` int(11) NOT NULL DEFAULT '0' COMMENT '0 обычный / 1 юрик',
  `birthday` date DEFAULT NULL,
  `memorial` varchar(254) DEFAULT NULL COMMENT 'Заметка при выдаче/продаже',
  `notes` text COMMENT 'Примечание',
  `is_regular` tinyint(1) NOT NULL DEFAULT '0',
  `is_dealer` tinyint(1) NOT NULL DEFAULT '0' COMMENT 'Является поставщиком?',
  `balance_enable` tinyint(1) NOT NULL DEFAULT '0',
  `prefer_cashless` tinyint(1) NOT NULL DEFAULT '0',
  `take_long` tinyint(1) NOT NULL DEFAULT '0' COMMENT 'Не забирает в срок технику с ремонта',
  `ignore_calls` tinyint(1) NOT NULL DEFAULT '0' COMMENT 'Игнорирует входящие вызовы',
  `is_bad` tinyint(1) NOT NULL DEFAULT '0',
  `is_realizator` tinyint(1) NOT NULL DEFAULT '0',
  `is_agent` tinyint(1) NOT NULL DEFAULT '0' COMMENT 'Посредник',
  `visit_source` int(11) DEFAULT '0' COMMENT 'Источник обращения',
  `photo_id` int(11) DEFAULT NULL,
  `INN` varchar(30) DEFAULT NULL,
  `KPP` varchar(20) DEFAULT NULL,
  `OGRN` varchar(20) DEFAULT NULL,
  `web_password` varchar(16) DEFAULT NULL COMMENT 'Пароль для проверки статусов ремонта',
  `ur_name` varchar(254) DEFAULT NULL COMMENT 'Наименование юридического лица',
  `email` varchar(254) DEFAULT NULL,
  `icq` varchar(20) DEFAULT NULL,
  `skype` varchar(254) DEFAULT NULL,
  `viber` varchar(45) DEFAULT NULL,
  `telegram` varchar(254) DEFAULT NULL,
  `site` varchar(254) DEFAULT NULL,
  `whatsapp` varchar(254) DEFAULT NULL,
  `agent_name` varchar(254) DEFAULT NULL,
  `agent_surname` varchar(254) DEFAULT NULL,
  `agent_patronymic` varchar(254) DEFAULT NULL,
  `agent_phone` varchar(254) DEFAULT NULL,
  `agent_phone_clean` varchar(45) DEFAULT NULL,
  `agent2_name` varchar(254) DEFAULT NULL,
  `agent2_surname` varchar(254) DEFAULT NULL,
  `agent2_patronymic` varchar(254) DEFAULT NULL,
  `agent2_phone` varchar(254) DEFAULT NULL,
  `agent2_phone_clean` varchar(45) DEFAULT NULL,
  `created` datetime DEFAULT NULL,
  `balance` decimal(19,4) NOT NULL DEFAULT '0.0000' COMMENT 'Баланс клиента',
  `price_col` int(11) NOT NULL DEFAULT '2' COMMENT '2 - розница, 3 - Опт, 4 - Опт2, 5 - Опт3',
  `repairs` int(11) NOT NULL DEFAULT '0' COMMENT 'Кол-во ремонтов',
  `purchases` int(11) NOT NULL DEFAULT '0' COMMENT 'Кол-во покупок',
  `token` char(64) DEFAULT NULL,
  `employee` int(11) DEFAULT NULL COMMENT 'id карточки сотрудника-клиента',
  `short_name` varchar(40) DEFAULT NULL,
  PRIMARY KEY (`id`),
  KEY `fk_clients_users1_idx` (`creator`),
  KEY `name` (`name`),
  KEY `surname` (`surname`),
  KEY `patronymic` (`patronymic`),
  KEY `agent_phone_clean` (`agent_phone_clean`),
  KEY `agent2_phone_clean` (`agent2_phone_clean`),
  KEY `FK_clients_visit_sources` (`visit_source`),
  KEY `fk_clients_users2` (`employee`),
  CONSTRAINT `FK_clients_visit_sources` FOREIGN KEY (`visit_source`) REFERENCES `visit_sources` (`id`) ON DELETE SET NULL ON UPDATE NO ACTION,
  CONSTRAINT `fk_clients_users1` FOREIGN KEY (`creator`) REFERENCES `users` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT `fk_clients_users2` FOREIGN KEY (`employee`) REFERENCES `users` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION
) ENGINE=InnoDB AUTO_INCREMENT=1953 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `clients`
--

LOCK TABLES `clients` WRITE;
/*!40000 ALTER TABLE `clients` DISABLE KEYS */;
INSERT INTO `clients` VALUES (111,1,'\"Центр обслуживания управления образования и молодежи и его подведомственных учреждений\" Муниципального образования Республики',NULL,NULL,1,1,NULL,NULL,NULL,NULL,NULL,1,0,NULL,NULL,NULL,0,0,0,0,0,0,0,0,0,5,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0.0000,2,0,0,NULL,NULL,NULL),(864,1,'',NULL,NULL,1,1,NULL,NULL,NULL,NULL,NULL,1,0,NULL,NULL,NULL,0,0,0,0,0,0,0,0,0,3,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,0.0000,2,0,0,NULL,NULL,NULL),(1952,1,'',NULL,NULL,1,1,NULL,NULL,NULL,NULL,NULL,1,0,NULL,NULL,NULL,0,0,1,0,0,0,0,0,0,5,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,1000.0000,2,0,0,NULL,NULL,NULL);
/*!40000 ALTER TABLE `clients` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `comments`
--

DROP TABLE IF EXISTS `comments`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `comments` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `text` text,
  `created` datetime DEFAULT NULL,
  `user` int(11) NOT NULL,
  `remont` int(11) DEFAULT NULL,
  `client` int(11) DEFAULT NULL,
  `task_id` int(11) DEFAULT NULL,
  `part_request` int(11) DEFAULT NULL,
  PRIMARY KEY (`id`),
  KEY `fk_comments_users1_idx` (`user`),
  KEY `fk_comments_workshop1_idx` (`remont`),
  KEY `FK_comments_clients` (`client`),
  KEY `FK_comments_tasks` (`task_id`),
  CONSTRAINT `FK_comments_clients` FOREIGN KEY (`client`) REFERENCES `clients` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT `FK_comments_tasks` FOREIGN KEY (`task_id`) REFERENCES `tasks` (`idt`) ON DELETE SET NULL ON UPDATE NO ACTION,
  CONSTRAINT `fk_comments_users1` FOREIGN KEY (`user`) REFERENCES `users` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT `fk_comments_workshop1` FOREIGN KEY (`remont`) REFERENCES `workshop` (`id`) ON DELETE CASCADE ON UPDATE NO ACTION
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `comments`
--

LOCK TABLES `comments` WRITE;
/*!40000 ALTER TABLE `comments` DISABLE KEYS */;
INSERT INTO `comments` VALUES (1,'aaaaa','2025-11-21 01:52:30',1,5,NULL,NULL,NULL),(2,'bbbbb','2025-11-21 01:52:30',1,5,NULL,NULL,NULL),(3,'ccccc','2025-11-21 01:52:30',1,5,NULL,NULL,NULL);
/*!40000 ALTER TABLE `comments` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `companies`
--

DROP TABLE IF EXISTS `companies`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `companies` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `type` int(11) DEFAULT NULL COMMENT '0 ООО\n1 ИП\n2 ЧП',
  `name` varchar(254) DEFAULT NULL,
  `inn` varchar(254) DEFAULT NULL,
  `kpp` varchar(254) DEFAULT NULL,
  `ogrn` varchar(254) DEFAULT NULL,
  `ur_address` varchar(254) DEFAULT NULL,
  `site` varchar(254) DEFAULT NULL,
  `email` varchar(254) DEFAULT NULL,
  `logo` mediumblob COMMENT 'Логотип оргпнизации',
  `banks` text,
  `is_default` tinyint(1) NOT NULL DEFAULT '0',
  `status` tinyint(1) NOT NULL DEFAULT '1' COMMENT '0 архивная\\n1 активная',
  `director` int(11) DEFAULT NULL,
  `accountant` int(11) DEFAULT NULL,
  `tax_form` int(11) NOT NULL DEFAULT '0' COMMENT 'система налогообложения',
  `description` varchar(254) DEFAULT NULL COMMENT 'Описание или примечание',
  PRIMARY KEY (`id`),
  KEY `fk_companies_users1_idx` (`director`),
  KEY `fk_companies_users2_idx` (`accountant`),
  CONSTRAINT `fk_companies_users1` FOREIGN KEY (`director`) REFERENCES `users` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT `fk_companies_users2` FOREIGN KEY (`accountant`) REFERENCES `users` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `companies`
--

LOCK TABLES `companies` WRITE;
/*!40000 ALTER TABLE `companies` DISABLE KEYS */;
INSERT INTO `companies` VALUES (1,1,'Сервисный центр',NULL,NULL,NULL,NULL,'service.com',NULL,NULL,NULL,1,1,1,1,0,NULL);
/*!40000 ALTER TABLE `companies` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `config`
--

DROP TABLE IF EXISTS `config`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `config` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `default_works_warranty` int(11) NOT NULL DEFAULT '6' COMMENT 'Гарантия по умолчанию в днях',
  `default_items_warranty` int(11) NOT NULL DEFAULT '6' COMMENT 'Гарантия по умолчанию в днях',
  `currency` char(3) DEFAULT NULL,
  `configcol` varchar(254) DEFAULT NULL,
  `email_config` text,
  `qs_print_pko` tinyint(1) NOT NULL DEFAULT '1',
  `qs_print_rko` tinyint(1) NOT NULL DEFAULT '1',
  `qs_print_pn` tinyint(1) NOT NULL DEFAULT '1',
  `qs_print_rn` tinyint(1) NOT NULL DEFAULT '1',
  `print_rep_stickers` tinyint(1) NOT NULL DEFAULT '0',
  `rep_stickers_copy` int(11) NOT NULL DEFAULT '2' COMMENT 'Кол-во стикеров при прёме техники в ремонт',
  `rep_auto_company` tinyint(1) NOT NULL DEFAULT '1',
  `config_modified` datetime DEFAULT NULL,
  `close_clients` tinyint(1) NOT NULL DEFAULT '0',
  `aster_host` varchar(254) DEFAULT NULL,
  `aster_port` int(11) NOT NULL,
  `aster_login` varchar(254) DEFAULT NULL,
  `aster_password` varchar(254) DEFAULT NULL,
  `aster_web_port` tinyint(4) NOT NULL DEFAULT '80',
  `time_zone` varchar(80) DEFAULT NULL,
  `realizator_enable` tinyint(1) NOT NULL DEFAULT '0',
  `online_store` int(11) DEFAULT NULL,
  `online_store_api` varchar(254) DEFAULT NULL,
  `online_store_key` varchar(300) DEFAULT NULL,
  `classic_kassa` tinyint(1) NOT NULL DEFAULT '0',
  `default_items_used_warranty` int(11) NOT NULL DEFAULT '6',
  `default_items_rep_warranty` int(11) NOT NULL DEFAULT '6',
  `default_items_razb_warranty` int(11) NOT NULL DEFAULT '6',
  `default_items_other_warranty` int(11) NOT NULL DEFAULT '6',
  `images_width` int(11) NOT NULL DEFAULT '1600',
  `item_img_limit` int(11) NOT NULL DEFAULT '3',
  `rep_img_limit` int(11) NOT NULL DEFAULT '5',
  `default_reserve_days` int(11) NOT NULL DEFAULT '3',
  `it_vis_opt` tinyint(1) NOT NULL DEFAULT '1',
  `it_vis_opt2` tinyint(1) NOT NULL DEFAULT '1',
  `it_vis_opt3` tinyint(1) NOT NULL DEFAULT '1',
  `it_vis_rozn` tinyint(1) NOT NULL DEFAULT '1',
  `it_vis_price_for_sc` tinyint(1) NOT NULL DEFAULT '1',
  `give2user_time` int(11) NOT NULL DEFAULT '3',
  `diag_accept_time` int(11) NOT NULL DEFAULT '3',
  `salary_day` int(11) NOT NULL DEFAULT '0',
  `parts_included` tinyint(1) NOT NULL DEFAULT '1' COMMENT 'стоимость запчастей включена в стоимость ремонта\n',
  `visit_source_force` tinyint(1) NOT NULL,
  `is_patronymic_required` tinyint(1) NOT NULL DEFAULT '0',
  `is_photo_required` tinyint(1) NOT NULL DEFAULT '0',
  `is_photo_out_req` tinyint(1) NOT NULL DEFAULT '0',
  `is_sn_req` tinyint(1) NOT NULL DEFAULT '0',
  `is_cartridge_sn_req` tinyint(1) NOT NULL DEFAULT '1' COMMENT 'Обязательное указание SN картриджа',
  `is_reason_req` tinyint(1) NOT NULL DEFAULT '0' COMMENT 'Обязательное "основание" в ПН',
  `address_required` tinyint(1) NOT NULL DEFAULT '0',
  `email_required` tinyint(1) NOT NULL DEFAULT '0',
  `phone_required` tinyint(1) NOT NULL DEFAULT '0',
  `diag_required` tinyint(1) NOT NULL DEFAULT '1',
  `is_master_set_on_new` tinyint(1) NOT NULL DEFAULT '0',
  `key` text,
  `print_warranty` tinyint(1) NOT NULL DEFAULT '1' COMMENT 'Настройки печати по умолчанию при выдаче йстройства',
  `print_works` tinyint(1) NOT NULL DEFAULT '1',
  `print_diagnostic` tinyint(1) NOT NULL DEFAULT '1',
  `print_reject` tinyint(1) NOT NULL DEFAULT '1',
  `print_check` tinyint(1) NOT NULL DEFAULT '0',
  `rko_on_pn` tinyint(1) NOT NULL DEFAULT '1' COMMENT 'Создание РКО на ПН по умолчанию',
  `card_close_time` int(11) NOT NULL DEFAULT '180' COMMENT 'Время автозакрытия карты ремонта после её открытия',
  `phone_mask1` varchar(20) NOT NULL DEFAULT '+0(000)000-00-00' COMMENT 'международный',
  `phone_mask2` varchar(20) NOT NULL DEFAULT '+0(0000)00-00-00' COMMENT 'городской в международном формате',
  `auto_switch_layout` tinyint(1) NOT NULL DEFAULT '1',
  `rep_price_by_manager` tinyint(1) NOT NULL DEFAULT '0',
  `video_codec` int(11) NOT NULL DEFAULT '1' COMMENT '1 mp4v, 2 h264',
  `print_new_repair_report` tinyint(1) NOT NULL DEFAULT '1',
  `statuses` text,
  `sms_config` text,
  `debt_rep_2_salary` tinyint(1) NOT NULL DEFAULT '0',
  `exchange_type` tinyint(1) NOT NULL DEFAULT '0' COMMENT '0 Auto, 1 manual',
  `exchange_source` int(11) NOT NULL DEFAULT '1' COMMENT '1 Yahoo',
  `exchange_rate` double NOT NULL DEFAULT '1',
  `history_by_sn` tinyint(1) NOT NULL DEFAULT '0' COMMENT 'Искать предыдущие ремонты только по SN',
  `clients_are_dealers` tinyint(1) NOT NULL DEFAULT '0' COMMENT 'Любой клиент - поставщик',
  `regular_customer_step` int(11) NOT NULL DEFAULT '0' COMMENT '0 отключено',
  `backup_enable` tinyint(1) NOT NULL DEFAULT '0',
  `backup_images` tinyint(1) NOT NULL DEFAULT '0',
  `backup_time` datetime NOT NULL DEFAULT '2017-01-01 18:00:00',
  `manual_maker` tinyint(1) NOT NULL DEFAULT '0' COMMENT 'добавление производителя на этапе приёмки',
  `TimeOfWork` text,
  `cartridge_enable` tinyint(1) NOT NULL DEFAULT '1',
  `vw_enable` tinyint(1) NOT NULL DEFAULT '0' COMMENT 'vendor warranty',
  `termsControl` tinyint(1) NOT NULL DEFAULT '0',
  `version` varchar(50) NOT NULL DEFAULT '2.1.5.123',
  `version_snap` varchar(50) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `config`
--

LOCK TABLES `config` WRITE;
/*!40000 ALTER TABLE `config` DISABLE KEYS */;
INSERT INTO `config` VALUES (1,0,0,'RUB',NULL,'{\"Host\":\"smtp.gmail.com\",\"Port\":465,\"Login\":\"\",\"Password\":\"\",\"EnableSsl\":true,\"EnableImplicitSsl\":false,\"Timeout\":60,\"Template\":null}',0,0,0,0,1,3,1,NULL,0,'',0,'admin','',80,'Russian Standard Time',1,NULL,NULL,NULL,1,0,0,0,0,1920,5,5,5,1,0,0,1,1,3,3,0,0,0,0,0,0,1,0,0,0,0,0,1,0,NULL,0,0,0,0,0,0,0,'+7 000 000-00-00','0-00-00',0,1,1,1,'[{\"Id\":0,\"Name\":\"Приём в ремонт\",\"Contains\":[1,33,11],\"Actions\":[],\"Roles\":[1,2,3,7,6,5,9],\"Color\":\"#00000000\",\"Terms\":\"01:00:00\"},{\"Id\":1,\"Name\":\"Проведение диагностики\",\"Contains\":[9,2],\"Actions\":[1,3],\"Roles\":[1,2,3,7,6,5,9],\"Color\":\"#00000000\",\"Terms\":\"05:00:00\"},{\"Id\":2,\"Name\":\"Ожидание решения клиента\",\"Contains\":[3,11],\"Actions\":null,\"Roles\":[1,5],\"Color\":\"#FFFFFF90\",\"Terms\":\"2.00:00:00\"},{\"Id\":3,\"Name\":\"Согласовано/детали получены\",\"Contains\":[4,32,5],\"Actions\":[2],\"Roles\":[1,3,6,5,9],\"Color\":\"#FFFFFF90\",\"Terms\":\"1.00:00:00\"},{\"Id\":4,\"Name\":\"Выполнение работ\",\"Contains\":[6,7,34],\"Actions\":[1,3],\"Roles\":[1,2,3,7,6,5,9],\"Color\":\"#00000000\",\"Terms\":\"5.00:00:00\"},{\"Id\":5,\"Name\":\"Ожидание комплектующих\",\"Contains\":[4],\"Actions\":null,\"Roles\":[1],\"Color\":\"#00000000\",\"Terms\":\"00:00:00\"},{\"Id\":6,\"Name\":\"Готово к выдаче\",\"Contains\":[8],\"Actions\":null,\"Roles\":[1,2,3,7,6,5,9],\"Color\":\"#FF90EE90\",\"Terms\":\"1.00:00:00\"},{\"Id\":7,\"Name\":\"Готово к выдаче без ремонта\",\"Contains\":[1,12],\"Actions\":null,\"Roles\":[1,2,3,7,6,5,9],\"Color\":\"#FF90EE90\",\"Terms\":\"1.00:00:00\"},{\"Id\":8,\"Name\":\"Выдано клиенту\",\"Contains\":[],\"Actions\":[2],\"Roles\":[1,3,6,5,9],\"Color\":\"#00000000\",\"Terms\":\"00:00:00\"},{\"Id\":9,\"Name\":\"Неисправность не проявилась\",\"Contains\":[7],\"Actions\":[],\"Roles\":[1,2,3,7,6,5,9],\"Color\":\"#00000000\",\"Terms\":\"1.00:00:00\"},{\"Id\":10,\"Name\":\"Смена офиса\",\"Contains\":null,\"Actions\":[],\"Roles\":[1,3,6,5,9],\"Color\":\"#00000000\",\"Terms\":\"1.00:00:00\"},{\"Id\":11,\"Name\":\"Отказ от ремонта\",\"Contains\":[7,34],\"Actions\":[2],\"Roles\":[1,3,6,5,9],\"Color\":\"#FFFFFF90\",\"Terms\":\"1.00:00:00\"},{\"Id\":12,\"Name\":\"Выдано клиенту без ремонта\",\"Contains\":[],\"Actions\":[2],\"Roles\":[1,3,6,5,9],\"Color\":\"#00000000\",\"Terms\":\"00:00:00\"},{\"Id\":13,\"Name\":\"Ожидает подтверждения\",\"Contains\":null,\"Actions\":[],\"Roles\":[1,2,3,6,5],\"Color\":\"#FFFFFF90\",\"Terms\":\"01:00:00\"},{\"Id\":14,\"Name\":\"Диагностика подтверждена\",\"Contains\":null,\"Actions\":[],\"Roles\":[1,2,3,7,6,5,9],\"Color\":\"#00000000\",\"Terms\":\"00:00:00\"},{\"Id\":15,\"Name\":\"Диагностика выполнена\",\"Contains\":[13],\"Actions\":[],\"Roles\":[1,2,3,7,6,5,9],\"Color\":\"#00000000\",\"Terms\":\"00:00:00\"},{\"Id\":16,\"Name\":\"Выдано в долг\",\"Contains\":[],\"Actions\":[],\"Roles\":[1,3,6,5,9],\"Color\":\"#00000000\",\"Terms\":\"00:00:00\"},{\"Id\":33,\"Name\":\"Разборка устройства\",\"Contains\":[34,35],\"Actions\":[],\"Roles\":[1,2,3,5,6,7,9],\"Color\":\"#00000000\",\"Terms\":\"01:00:00\"},{\"Id\":34,\"Name\":\"Сборка/тестирование устройства\",\"Contains\":[4,6,7,13],\"Actions\":[1],\"Roles\":[1,2,3,5,6,7,9],\"Color\":\"#00000000\",\"Terms\":\"03:00:00\"},{\"Id\":35,\"Name\":\"Устройство разобрано\",\"Contains\":[1,34],\"Actions\":[],\"Roles\":[1,2,3,5,6,7,9],\"Color\":\"#00000000\",\"Terms\":\"02:00:00\"},{\"Id\":32,\"Name\":\"Ожидание комплектующих (Китай)\",\"Contains\":[4],\"Actions\":null,\"Roles\":[1,2,3,5,6,7,9],\"Color\":\"#00000000\",\"Terms\":\"60.00:00:00\"},{\"Id\":36,\"Name\":\"Заказ запчастей\",\"Contains\":[38],\"Actions\":[1],\"Roles\":[1,3,5,6,9],\"Color\":\"#00000000\",\"Terms\":\"01:00:00\"},{\"Id\":38,\"Name\":\"Ожидание запчастей (Китай)\",\"Contains\":[39],\"Actions\":[],\"Roles\":[1,3,5,6,9],\"Color\":\"#00000000\",\"Terms\":\"60.00:00:00\"},{\"Id\":39,\"Name\":\"Выдача заказанных запчастей\",\"Contains\":[8],\"Actions\":null,\"Roles\":[1,5,6],\"Color\":\"#00000000\",\"Terms\":\"2.00:00:00\"}]',NULL,1,0,-1,89,1,1,0,0,0,'2017-01-01 18:00:00',1,'{\r\n  \"WeekDays\": [\r\n    {\r\n      \"$type\": \"ASC.Objects.WorkDay, ASC\",\r\n      \"DayOfWeek\": 0,\r\n      \"IsWorkday\": false,\r\n      \"OpenHours\": 10,\r\n      \"OpenMinutes\": 0,\r\n      \"CloseHours\": 19,\r\n      \"CloseMinutes\": 0\r\n    },\r\n    {\r\n      \"$type\": \"ASC.Objects.WorkDay, ASC\",\r\n      \"DayOfWeek\": 1,\r\n      \"IsWorkday\": true,\r\n      \"OpenHours\": 10,\r\n      \"OpenMinutes\": 30,\r\n      \"CloseHours\": 19,\r\n      \"CloseMinutes\": 30\r\n    },\r\n    {\r\n      \"$type\": \"ASC.Objects.WorkDay, ASC\",\r\n      \"DayOfWeek\": 2,\r\n      \"IsWorkday\": true,\r\n      \"OpenHours\": 10,\r\n      \"OpenMinutes\": 30,\r\n      \"CloseHours\": 19,\r\n      \"CloseMinutes\": 30\r\n    },\r\n    {\r\n      \"$type\": \"ASC.Objects.WorkDay, ASC\",\r\n      \"DayOfWeek\": 3,\r\n      \"IsWorkday\": true,\r\n      \"OpenHours\": 10,\r\n      \"OpenMinutes\": 30,\r\n      \"CloseHours\": 19,\r\n      \"CloseMinutes\": 30\r\n    },\r\n    {\r\n      \"$type\": \"ASC.Objects.WorkDay, ASC\",\r\n      \"DayOfWeek\": 4,\r\n      \"IsWorkday\": true,\r\n      \"OpenHours\": 10,\r\n      \"OpenMinutes\": 30,\r\n      \"CloseHours\": 19,\r\n      \"CloseMinutes\": 30\r\n    },\r\n    {\r\n      \"$type\": \"ASC.Objects.WorkDay, ASC\",\r\n      \"DayOfWeek\": 5,\r\n      \"IsWorkday\": true,\r\n      \"OpenHours\": 10,\r\n      \"OpenMinutes\": 30,\r\n      \"CloseHours\": 19,\r\n      \"CloseMinutes\": 30\r\n    },\r\n    {\r\n      \"$type\": \"ASC.Objects.WorkDay, ASC\",\r\n      \"DayOfWeek\": 6,\r\n      \"IsWorkday\": true,\r\n      \"OpenHours\": 12,\r\n      \"OpenMinutes\": 0,\r\n      \"CloseHours\": 18,\r\n      \"CloseMinutes\": 0\r\n    }\r\n  ],\r\n  \"Holidays\": []\r\n}',0,0,0,'3.7.37.1184','0.0.0.304');
/*!40000 ALTER TABLE `config` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `currency`
--

DROP TABLE IF EXISTS `currency`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `currency` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `from` char(3) NOT NULL,
  `to` char(3) NOT NULL,
  `rate` decimal(19,4) NOT NULL,
  `created` datetime NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `currency`
--

LOCK TABLES `currency` WRITE;
/*!40000 ALTER TABLE `currency` DISABLE KEYS */;
/*!40000 ALTER TABLE `currency` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `dealer_payments`
--

DROP TABLE IF EXISTS `dealer_payments`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `dealer_payments` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `payment_date` datetime NOT NULL,
  `user_id` int(11) NOT NULL DEFAULT '0',
  `state` int(11) NOT NULL DEFAULT '0',
  `dealer_id` int(11) NOT NULL DEFAULT '0',
  `summ` decimal(19,4) NOT NULL DEFAULT '0.0000',
  PRIMARY KEY (`id`),
  KEY `FK_dealer_payments_users` (`user_id`),
  KEY `FK_dealer_payments_clients` (`dealer_id`),
  CONSTRAINT `FK_dealer_payments_clients` FOREIGN KEY (`dealer_id`) REFERENCES `clients` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT `FK_dealer_payments_users` FOREIGN KEY (`user_id`) REFERENCES `users` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `dealer_payments`
--

LOCK TABLES `dealer_payments` WRITE;
/*!40000 ALTER TABLE `dealer_payments` DISABLE KEYS */;
/*!40000 ALTER TABLE `dealer_payments` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `device_makers`
--

DROP TABLE IF EXISTS `device_makers`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `device_makers` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(254) DEFAULT NULL,
  PRIMARY KEY (`id`),
  KEY `name` (`name`)
) ENGINE=InnoDB AUTO_INCREMENT=44 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `device_makers`
--

LOCK TABLES `device_makers` WRITE;
/*!40000 ALTER TABLE `device_makers` DISABLE KEYS */;
INSERT INTO `device_makers` VALUES (1,' '),(2,'Acer'),(3,'AOC'),(4,'Apple'),(5,'Aquarius'),(6,'Asus'),(7,'BenQ'),(8,'BlackBerry'),(9,'Clevo'),(10,'Creative'),(11,'Dell'),(12,'DNS'),(13,'Doogee'),(14,'eMachines'),(15,'Fly'),(16,'Fujitsu-Siemens'),(17,'Gigabyte'),(18,'Honor'),(19,'HP'),(20,'HTC'),(21,'Huawei'),(22,'Irbis'),(23,'iRu'),(24,'Lenovo'),(25,'LG'),(26,'Meizu'),(27,'Motorola'),(28,'MSI'),(29,'NEC'),(30,'Nokia'),(31,'OPPO'),(32,'Packard Bell'),(33,'Panasonic'),(34,'Philips'),(35,'Pipo'),(36,'Razer'),(37,'Samsung'),(38,'Sony'),(39,'SUPRA'),(40,'Toshiba'),(41,'ViewSonic'),(42,'Xerox'),(43,'Xiaomi');
/*!40000 ALTER TABLE `device_makers` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `device_models`
--

DROP TABLE IF EXISTS `device_models`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `device_models` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(254) DEFAULT NULL,
  `position` int(11) DEFAULT NULL,
  `maker` int(11) NOT NULL,
  `device` int(11) DEFAULT NULL,
  PRIMARY KEY (`id`),
  KEY `fk_device_models_device_makers1_idx` (`maker`),
  KEY `fk_device_models_devices1_idx` (`device`),
  KEY `name` (`name`),
  CONSTRAINT `fk_device_models_device_makers1` FOREIGN KEY (`maker`) REFERENCES `device_makers` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT `fk_device_models_devices1` FOREIGN KEY (`device`) REFERENCES `devices` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION
) ENGINE=InnoDB AUTO_INCREMENT=7 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `device_models`
--

LOCK TABLES `device_models` WRITE;
/*!40000 ALTER TABLE `device_models` DISABLE KEYS */;
INSERT INTO `device_models` VALUES (1,'5742G',NULL,2,1),(2,'2043BW',NULL,37,2),(3,'IP710',NULL,13,3),(4,'iPhone X',NULL,4,4),(5,'Phaser 3010',NULL,42,5),(6,'Гравицапа ГЦ-1000М',NULL,1,6);
/*!40000 ALTER TABLE `device_models` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `devices`
--

DROP TABLE IF EXISTS `devices`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `devices` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(254) DEFAULT NULL,
  `position` int(11) DEFAULT NULL,
  `enable` tinyint(1) DEFAULT '1',
  `refill` tinyint(1) NOT NULL DEFAULT '0',
  `fault_list` text,
  `look_list` text,
  `complect_list` text,
  `company_list` text,
  PRIMARY KEY (`id`),
  KEY `name` (`name`)
) ENGINE=InnoDB AUTO_INCREMENT=7 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `devices`
--

LOCK TABLES `devices` WRITE;
/*!40000 ALTER TABLE `devices` DISABLE KEYS */;
INSERT INTO `devices` VALUES (1,'ноутбук',1,1,0,'не включается,нет инициализации,не загружается ОС,перегревается,шумит вентилятор системы охлаждения,выключается,разбит ЖКД','хорошее состояние,удовлетворительное состояние,потёртости на крышке,плохое состояние','ноутбук,зарядное устройство,аккумулятор,сумка','1,21,32\r\n'),(2,'монитор',2,1,0,'не включается,искажено изображение,выключается','хорошее состояние,удовлетворительное состояние,есть следы механического повреждения','монитор,подставка,без подставки,блок питания','1,10,14,21,22,32\r\n'),(3,'планшетный компьютер',3,1,0,'не включается,не заряжает батарею,разбит сенсор,поврежден дисплей','хорошее состояние,удовлетворительное состояние,плохое состояние, потёртости','планшет,чехол,зарядное устройство,шнур USB','1,2,6,9,12,16,17,19,21,23,26,30,32,33\r\n'),(4,'мобильный телефон',4,1,0,'разбит дисплей,не включается,не работает микрофон,не работает динамик,быстро разряжается батарея','хорошее состояние,удовлетворительное состояние,имеются следы активной эксплуатации,имеются следы механического воздействия (удара)','телефон,лоток SIM-карты,чехол,зарядное устройство,SIM-карта,карта памяти','1,9,13,21,23,30,31,32,40\r\n'),(5,'картридж',5,1,1,'','','',''),(6,'устройство специализированное',6,1,0,'','','','');
/*!40000 ALTER TABLE `devices` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `doc_templates`
--

DROP TABLE IF EXISTS `doc_templates`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `doc_templates` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(50) NOT NULL,
  `description` varchar(254) DEFAULT NULL,
  `data` mediumblob NOT NULL,
  `checksum` binary(20) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=30 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `doc_templates`
--

LOCK TABLES `doc_templates` WRITE;
/*!40000 ALTER TABLE `doc_templates` DISABLE KEYS */;
/*!40000 ALTER TABLE `doc_templates` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `doc_templates_snap`
--

DROP TABLE IF EXISTS `doc_templates_snap`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `doc_templates_snap` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(50) NOT NULL,
  `description` varchar(254) DEFAULT NULL,
  `data` mediumblob NOT NULL,
  `checksum` binary(20) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=32 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `doc_templates_snap`
--

LOCK TABLES `doc_templates_snap` WRITE;
/*!40000 ALTER TABLE `doc_templates_snap` DISABLE KEYS */;
/*!40000 ALTER TABLE `doc_templates_snap` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `docs`
--

DROP TABLE IF EXISTS `docs`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `docs` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `type` int(11) NOT NULL DEFAULT '0' COMMENT '1 Приходная накладная\r\n2 Расходная накладная\r\n3 Списание товара\r\n4 Накладные перемещения\r\n5 счета\r\n6 резерв товара\r\n8 выкуп техники',
  `state` int(11) DEFAULT '0' COMMENT 'Состояние\r\n0 не активна\r\n1 активна\r\n2 ПН проведена (создан РКО)\r\n3 Резерв товара\r\n4 резерв снят\r\n5 РН проведена (создан ПКО)\r\n6 списание товара\r\n6 Списание товара ОК\r\n7 РН распроведена',
  `is_realization` tinyint(1) NOT NULL DEFAULT '0',
  `payment_system` int(11) NOT NULL DEFAULT '0',
  `company` int(11) NOT NULL,
  `store` int(11) DEFAULT NULL,
  `user` int(11) NOT NULL,
  `total` decimal(19,4) NOT NULL,
  `notes` varchar(1000) DEFAULT NULL,
  `created` datetime NOT NULL,
  `updated_at` datetime DEFAULT NULL,
  `office` int(11) NOT NULL,
  `dealer` int(11) DEFAULT NULL,
  `currency_rate` decimal(19,4) DEFAULT NULL,
  `img1` mediumblob,
  `img2` mediumblob,
  `img3` mediumblob,
  `reason` varchar(254) DEFAULT NULL COMMENT 'Основание',
  `order_id` int(11) DEFAULT NULL COMMENT 'id кассового ордера',
  `price_option` int(11) NOT NULL DEFAULT '1',
  `return_percent` int(11) NOT NULL DEFAULT '0' COMMENT 'процент с прибыли отдаваемый реализатору',
  `reserve_days` int(11) NOT NULL DEFAULT '0',
  `master_id` int(11) DEFAULT NULL,
  `repair_id` int(11) DEFAULT NULL,
  `works_included` tinyint(1) NOT NULL DEFAULT '0' COMMENT 'Стоимость работ включена в стоимость товара',
  `invoice` int(11) DEFAULT NULL COMMENT 'номер счёта',
  `track` varchar(50) DEFAULT NULL COMMENT 'Номер ТТН',
  `d_store` int(11) DEFAULT NULL,
  `d_pay` tinyint(1) NOT NULL DEFAULT '0' COMMENT 'Списание средств с офиса на который делается перемещение',
  PRIMARY KEY (`id`),
  KEY `fk_docs_companies1_idx` (`company`),
  KEY `fk_docs_offices1_idx` (`office`),
  KEY `fk_docs_clients1_idx` (`dealer`),
  KEY `fk_docs_users1_idx` (`user`),
  KEY `fk_docs_stores1_idx` (`store`),
  KEY `fk_docs_cash_orders1_idx` (`order_id`),
  KEY `fk_docs_users2_idx` (`master_id`),
  KEY `fk_docs_workshop1_idx` (`repair_id`),
  KEY `FK_docs_payment_systems` (`payment_system`),
  KEY `created` (`created`),
  KEY `FK_docs_invoice` (`invoice`),
  KEY `FK_docs_stores` (`d_store`),
  CONSTRAINT `FK_docs_invoice` FOREIGN KEY (`invoice`) REFERENCES `invoice` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT `FK_docs_payment_systems` FOREIGN KEY (`payment_system`) REFERENCES `payment_systems` (`system_id`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT `FK_docs_stores` FOREIGN KEY (`d_store`) REFERENCES `stores` (`id`) ON DELETE SET NULL ON UPDATE CASCADE,
  CONSTRAINT `fk_docs_cash_orders1` FOREIGN KEY (`order_id`) REFERENCES `cash_orders` (`id`) ON DELETE CASCADE ON UPDATE NO ACTION,
  CONSTRAINT `fk_docs_clients1` FOREIGN KEY (`dealer`) REFERENCES `clients` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT `fk_docs_companies1` FOREIGN KEY (`company`) REFERENCES `companies` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT `fk_docs_offices1` FOREIGN KEY (`office`) REFERENCES `offices` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT `fk_docs_stores1` FOREIGN KEY (`store`) REFERENCES `stores` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT `fk_docs_users1` FOREIGN KEY (`user`) REFERENCES `users` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT `fk_docs_users2` FOREIGN KEY (`master_id`) REFERENCES `users` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT `fk_docs_workshop1` FOREIGN KEY (`repair_id`) REFERENCES `workshop` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION
) ENGINE=InnoDB AUTO_INCREMENT=3 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `docs`
--

LOCK TABLES `docs` WRITE;
/*!40000 ALTER TABLE `docs` DISABLE KEYS */;
INSERT INTO `docs` VALUES (1,0,0,0,0,1,1,1,522.0000,NULL,'2000-01-01 09:00:00',NULL,1,NULL,NULL,NULL,NULL,NULL,NULL,NULL,1,0,0,NULL,NULL,0,NULL,NULL,NULL,0),(2,0,0,0,0,1,1,1,4876.0000,NULL,'2000-01-01 09:00:00',NULL,1,NULL,NULL,NULL,NULL,NULL,NULL,NULL,1,0,0,NULL,NULL,0,NULL,NULL,NULL,0);
/*!40000 ALTER TABLE `docs` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `enumerator`
--

DROP TABLE IF EXISTS `enumerator`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `enumerator` (
  `number` int(11) DEFAULT '0'
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `enumerator`
--

LOCK TABLES `enumerator` WRITE;
/*!40000 ALTER TABLE `enumerator` DISABLE KEYS */;
INSERT INTO `enumerator` VALUES (-1000),(-999),(-998),(-997),(-996),(-995),(-994),(-993),(-992),(-991),(-990),(-989),(-988),(-987),(-986),(-985),(-984),(-983),(-982),(-981),(-980),(-979),(-978),(-977),(-976),(-975),(-974),(-973),(-972),(-971),(-970),(-969),(-968),(-967),(-966),(-965),(-964),(-963),(-962),(-961),(-960),(-959),(-958),(-957),(-956),(-955),(-954),(-953),(-952),(-951),(-950),(-949),(-948),(-947),(-946),(-945),(-944),(-943),(-942),(-941),(-940),(-939),(-938),(-937),(-936),(-935),(-934),(-933),(-932),(-931),(-930),(-929),(-928),(-927),(-926),(-925),(-924),(-923),(-922),(-921),(-920),(-919),(-918),(-917),(-916),(-915),(-914),(-913),(-912),(-911),(-910),(-909),(-908),(-907),(-906),(-905),(-904),(-903),(-902),(-901),(-900),(-899),(-898),(-897),(-896),(-895),(-894),(-893),(-892),(-891),(-890),(-889),(-888),(-887),(-886),(-885),(-884),(-883),(-882),(-881),(-880),(-879),(-878),(-877),(-876),(-875),(-874),(-873),(-872),(-871),(-870),(-869),(-868),(-867),(-866),(-865),(-864),(-863),(-862),(-861),(-860),(-859),(-858),(-857),(-856),(-855),(-854),(-853),(-852),(-851),(-850),(-849),(-848),(-847),(-846),(-845),(-844),(-843),(-842),(-841),(-840),(-839),(-838),(-837),(-836),(-835),(-834),(-833),(-832),(-831),(-830),(-829),(-828),(-827),(-826),(-825),(-824),(-823),(-822),(-821),(-820),(-819),(-818),(-817),(-816),(-815),(-814),(-813),(-812),(-811),(-810),(-809),(-808),(-807),(-806),(-805),(-804),(-803),(-802),(-801),(-800),(-799),(-798),(-797),(-796),(-795),(-794),(-793),(-792),(-791),(-790),(-789),(-788),(-787),(-786),(-785),(-784),(-783),(-782),(-781),(-780),(-779),(-778),(-777),(-776),(-775),(-774),(-773),(-772),(-771),(-770),(-769),(-768),(-767),(-766),(-765),(-764),(-763),(-762),(-761),(-760),(-759),(-758),(-757),(-756),(-755),(-754),(-753),(-752),(-751),(-750),(-749),(-748),(-747),(-746),(-745),(-744),(-743),(-742),(-741),(-740),(-739),(-738),(-737),(-736),(-735),(-734),(-733),(-732),(-731),(-730),(-729),(-728),(-727),(-726),(-725),(-724),(-723),(-722),(-721),(-720),(-719),(-718),(-717),(-716),(-715),(-714),(-713),(-712),(-711),(-710),(-709),(-708),(-707),(-706),(-705),(-704),(-703),(-702),(-701),(-700),(-699),(-698),(-697),(-696),(-695),(-694),(-693),(-692),(-691),(-690),(-689),(-688),(-687),(-686),(-685),(-684),(-683),(-682),(-681),(-680),(-679),(-678),(-677),(-676),(-675),(-674),(-673),(-672),(-671),(-670),(-669),(-668),(-667),(-666),(-665),(-664),(-663),(-662),(-661),(-660),(-659),(-658),(-657),(-656),(-655),(-654),(-653),(-652),(-651),(-650),(-649),(-648),(-647),(-646),(-645),(-644),(-643),(-642),(-641),(-640),(-639),(-638),(-637),(-636),(-635),(-634),(-633),(-632),(-631),(-630),(-629),(-628),(-627),(-626),(-625),(-624),(-623),(-622),(-621),(-620),(-619),(-618),(-617),(-616),(-615),(-614),(-613),(-612),(-611),(-610),(-609),(-608),(-607),(-606),(-605),(-604),(-603),(-602),(-601),(-600),(-599),(-598),(-597),(-596),(-595),(-594),(-593),(-592),(-591),(-590),(-589),(-588),(-587),(-586),(-585),(-584),(-583),(-582),(-581),(-580),(-579),(-578),(-577),(-576),(-575),(-574),(-573),(-572),(-571),(-570),(-569),(-568),(-567),(-566),(-565),(-564),(-563),(-562),(-561),(-560),(-559),(-558),(-557),(-556),(-555),(-554),(-553),(-552),(-551),(-550),(-549),(-548),(-547),(-546),(-545),(-544),(-543),(-542),(-541),(-540),(-539),(-538),(-537),(-536),(-535),(-534),(-533),(-532),(-531),(-530),(-529),(-528),(-527),(-526),(-525),(-524),(-523),(-522),(-521),(-520),(-519),(-518),(-517),(-516),(-515),(-514),(-513),(-512),(-511),(-510),(-509),(-508),(-507),(-506),(-505),(-504),(-503),(-502),(-501),(-500),(-499),(-498),(-497),(-496),(-495),(-494),(-493),(-492),(-491),(-490),(-489),(-488),(-487),(-486),(-485),(-484),(-483),(-482),(-481),(-480),(-479),(-478),(-477),(-476),(-475),(-474),(-473),(-472),(-471),(-470),(-469),(-468),(-467),(-466),(-465),(-464),(-463),(-462),(-461),(-460),(-459),(-458),(-457),(-456),(-455),(-454),(-453),(-452),(-451),(-450),(-449),(-448),(-447),(-446),(-445),(-444),(-443),(-442),(-441),(-440),(-439),(-438),(-437),(-436),(-435),(-434),(-433),(-432),(-431),(-430),(-429),(-428),(-427),(-426),(-425),(-424),(-423),(-422),(-421),(-420),(-419),(-418),(-417),(-416),(-415),(-414),(-413),(-412),(-411),(-410),(-409),(-408),(-407),(-406),(-405),(-404),(-403),(-402),(-401),(-400),(-399),(-398),(-397),(-396),(-395),(-394),(-393),(-392),(-391),(-390),(-389),(-388),(-387),(-386),(-385),(-384),(-383),(-382),(-381),(-380),(-379),(-378),(-377),(-376),(-375),(-374),(-373),(-372),(-371),(-370),(-369),(-368),(-367),(-366),(-365),(-364),(-363),(-362),(-361),(-360),(-359),(-358),(-357),(-356),(-355),(-354),(-353),(-352),(-351),(-350),(-349),(-348),(-347),(-346),(-345),(-344),(-343),(-342),(-341),(-340),(-339),(-338),(-337),(-336),(-335),(-334),(-333),(-332),(-331),(-330),(-329),(-328),(-327),(-326),(-325),(-324),(-323),(-322),(-321),(-320),(-319),(-318),(-317),(-316),(-315),(-314),(-313),(-312),(-311),(-310),(-309),(-308),(-307),(-306),(-305),(-304),(-303),(-302),(-301),(-300),(-299),(-298),(-297),(-296),(-295),(-294),(-293),(-292),(-291),(-290),(-289),(-288),(-287),(-286),(-285),(-284),(-283),(-282),(-281),(-280),(-279),(-278),(-277),(-276),(-275),(-274),(-273),(-272),(-271),(-270),(-269),(-268),(-267),(-266),(-265),(-264),(-263),(-262),(-261),(-260),(-259),(-258),(-257),(-256),(-255),(-254),(-253),(-252),(-251),(-250),(-249),(-248),(-247),(-246),(-245),(-244),(-243),(-242),(-241),(-240),(-239),(-238),(-237),(-236),(-235),(-234),(-233),(-232),(-231),(-230),(-229),(-228),(-227),(-226),(-225),(-224),(-223),(-222),(-221),(-220),(-219),(-218),(-217),(-216),(-215),(-214),(-213),(-212),(-211),(-210),(-209),(-208),(-207),(-206),(-205),(-204),(-203),(-202),(-201),(-200),(-199),(-198),(-197),(-196),(-195),(-194),(-193),(-192),(-191),(-190),(-189),(-188),(-187),(-186),(-185),(-184),(-183),(-182),(-181),(-180),(-179),(-178),(-177),(-176),(-175),(-174),(-173),(-172),(-171),(-170),(-169),(-168),(-167),(-166),(-165),(-164),(-163),(-162),(-161),(-160),(-159),(-158),(-157),(-156),(-155),(-154),(-153),(-152),(-151),(-150),(-149),(-148),(-147),(-146),(-145),(-144),(-143),(-142),(-141),(-140),(-139),(-138),(-137),(-136),(-135),(-134),(-133),(-132),(-131),(-130),(-129),(-128),(-127),(-126),(-125),(-124),(-123),(-122),(-121),(-120),(-119),(-118),(-117),(-116),(-115),(-114),(-113),(-112),(-111),(-110),(-109),(-108),(-107),(-106),(-105),(-104),(-103),(-102),(-101),(-100),(-99),(-98),(-97),(-96),(-95),(-94),(-93),(-92),(-91),(-90),(-89),(-88),(-87),(-86),(-85),(-84),(-83),(-82),(-81),(-80),(-79),(-78),(-77),(-76),(-75),(-74),(-73),(-72),(-71),(-70),(-69),(-68),(-67),(-66),(-65),(-64),(-63),(-62),(-61),(-60),(-59),(-58),(-57),(-56),(-55),(-54),(-53),(-52),(-51),(-50),(-49),(-48),(-47),(-46),(-45),(-44),(-43),(-42),(-41),(-40),(-39),(-38),(-37),(-36),(-35),(-34),(-33),(-32),(-31),(-30),(-29),(-28),(-27),(-26),(-25),(-24),(-23),(-22),(-21),(-20),(-19),(-18),(-17),(-16),(-15),(-14),(-13),(-12),(-11),(-10),(-9),(-8),(-7),(-6),(-5),(-4),(-3),(-2),(-1);
/*!40000 ALTER TABLE `enumerator` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `export_cat_match`
--

DROP TABLE IF EXISTS `export_cat_match`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `export_cat_match` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `local_id` int(11) NOT NULL,
  `remote_id` int(11) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `export_cat_match`
--

LOCK TABLES `export_cat_match` WRITE;
/*!40000 ALTER TABLE `export_cat_match` DISABLE KEYS */;
/*!40000 ALTER TABLE `export_cat_match` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `export_items`
--

DROP TABLE IF EXISTS `export_items`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `export_items` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `item_id` int(11) NOT NULL,
  `state` int(11) NOT NULL DEFAULT '0',
  `sync_datetime` datetime NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `export_items`
--

LOCK TABLES `export_items` WRITE;
/*!40000 ALTER TABLE `export_items` DISABLE KEYS */;
/*!40000 ALTER TABLE `export_items` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `extensions`
--

DROP TABLE IF EXISTS `extensions`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `extensions` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `context` varchar(40) NOT NULL,
  `exten` varchar(50) NOT NULL,
  `priority` int(11) NOT NULL,
  `app` varchar(40) NOT NULL,
  `appdata` text NOT NULL,
  `action_id` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`),
  UNIQUE KEY `id` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `extensions`
--

LOCK TABLES `extensions` WRITE;
/*!40000 ALTER TABLE `extensions` DISABLE KEYS */;
/*!40000 ALTER TABLE `extensions` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `field_values`
--

DROP TABLE IF EXISTS `field_values`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `field_values` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `field_id` int(11) NOT NULL,
  `repair_id` int(11) DEFAULT NULL,
  `item_id` int(11) DEFAULT NULL,
  `value` text,
  PRIMARY KEY (`id`),
  KEY `FK_field_values_workshop` (`repair_id`),
  KEY `FK_field_values_fields` (`field_id`),
  KEY `FK_field_values_store_items` (`item_id`),
  CONSTRAINT `FK_field_values_fields` FOREIGN KEY (`field_id`) REFERENCES `fields` (`id`) ON DELETE CASCADE ON UPDATE NO ACTION,
  CONSTRAINT `FK_field_values_store_items` FOREIGN KEY (`item_id`) REFERENCES `store_items` (`id`) ON DELETE CASCADE ON UPDATE SET NULL,
  CONSTRAINT `FK_field_values_workshop` FOREIGN KEY (`repair_id`) REFERENCES `workshop` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION
) ENGINE=InnoDB AUTO_INCREMENT=241 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `field_values`
--

LOCK TABLES `field_values` WRITE;
/*!40000 ALTER TABLE `field_values` DISABLE KEYS */;
/*!40000 ALTER TABLE `field_values` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `fields`
--

DROP TABLE IF EXISTS `fields`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `fields` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `_f` tinyint(1) NOT NULL DEFAULT '0' COMMENT '0 ремонты; 1 товары',
  `name` varchar(254) NOT NULL,
  `type` int(11) NOT NULL,
  `def_values` text,
  `required` tinyint(1) NOT NULL DEFAULT '0',
  `printable` tinyint(1) NOT NULL DEFAULT '0',
  `updated_at` datetime DEFAULT NULL,
  `devices` text,
  `categories` text,
  `archive` tinyint(1) NOT NULL DEFAULT '0',
  `dev_match` tinyint(1) NOT NULL DEFAULT '0' COMMENT '1 - производить поиск совпадений при приёме в ремонт',
  `input_mask` tinytext,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=6 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `fields`
--

LOCK TABLES `fields` WRITE;
/*!40000 ALTER TABLE `fields` DISABLE KEYS */;
/*!40000 ALTER TABLE `fields` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `hooks`
--

DROP TABLE IF EXISTS `hooks`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `hooks` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `hook_id` int(10) unsigned NOT NULL DEFAULT '0',
  `status` int(10) unsigned NOT NULL DEFAULT '0',
  `created_at` datetime DEFAULT NULL,
  `updated_at` datetime DEFAULT NULL,
  `event` varchar(50) DEFAULT NULL,
  `p0` int(10) unsigned DEFAULT NULL,
  `prm` text,
  `task_id` int(11) DEFAULT NULL,
  PRIMARY KEY (`id`),
  KEY `hook_id` (`hook_id`),
  KEY `status` (`status`),
  KEY `event` (`event`),
  KEY `created_at` (`created_at`),
  KEY `p0` (`p0`),
  KEY `FK_hooks_tasks` (`task_id`),
  CONSTRAINT `FK_hooks_tasks` FOREIGN KEY (`task_id`) REFERENCES `tasks` (`idt`) ON DELETE SET NULL ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `hooks`
--

LOCK TABLES `hooks` WRITE;
/*!40000 ALTER TABLE `hooks` DISABLE KEYS */;
/*!40000 ALTER TABLE `hooks` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `iaxfriends`
--

DROP TABLE IF EXISTS `iaxfriends`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `iaxfriends` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(40) NOT NULL,
  `type` enum('friend','user','peer') DEFAULT NULL,
  `username` varchar(40) DEFAULT NULL,
  `mailbox` varchar(40) DEFAULT NULL,
  `secret` varchar(40) DEFAULT NULL,
  `dbsecret` varchar(40) DEFAULT NULL,
  `context` varchar(40) DEFAULT NULL,
  `regcontext` varchar(40) DEFAULT NULL,
  `host` varchar(40) DEFAULT NULL,
  `ipaddr` varchar(40) DEFAULT NULL,
  `port` int(11) DEFAULT NULL,
  `defaultip` varchar(20) DEFAULT NULL,
  `sourceaddress` varchar(20) DEFAULT NULL,
  `mask` varchar(20) DEFAULT NULL,
  `regexten` varchar(40) DEFAULT NULL,
  `regseconds` int(11) DEFAULT NULL,
  `accountcode` varchar(80) DEFAULT NULL,
  `mohinterpret` varchar(20) DEFAULT NULL,
  `mohsuggest` varchar(20) DEFAULT NULL,
  `inkeys` varchar(40) DEFAULT NULL,
  `outkeys` varchar(40) DEFAULT NULL,
  `language` varchar(10) DEFAULT NULL,
  `callerid` varchar(100) DEFAULT NULL,
  `cid_number` varchar(40) DEFAULT NULL,
  `sendani` enum('yes','no') DEFAULT NULL,
  `fullname` varchar(40) DEFAULT NULL,
  `trunk` enum('yes','no') DEFAULT NULL,
  `auth` varchar(20) DEFAULT NULL,
  `maxauthreq` int(11) DEFAULT NULL,
  `requirecalltoken` enum('yes','no','auto') DEFAULT NULL,
  `encryption` enum('yes','no','aes128') DEFAULT NULL,
  `transfer` enum('yes','no','mediaonly') DEFAULT NULL,
  `jitterbuffer` enum('yes','no') DEFAULT NULL,
  `forcejitterbuffer` enum('yes','no') DEFAULT NULL,
  `disallow` varchar(200) DEFAULT NULL,
  `allow` varchar(200) DEFAULT NULL,
  `codecpriority` varchar(40) DEFAULT NULL,
  `qualify` varchar(10) DEFAULT NULL,
  `qualifysmoothing` enum('yes','no') DEFAULT NULL,
  `qualifyfreqok` varchar(10) DEFAULT NULL,
  `qualifyfreqnotok` varchar(10) DEFAULT NULL,
  `timezone` varchar(20) DEFAULT NULL,
  `adsi` enum('yes','no') DEFAULT NULL,
  `amaflags` varchar(20) DEFAULT NULL,
  `setvar` varchar(200) DEFAULT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `name` (`name`),
  KEY `iaxfriends_name` (`name`),
  KEY `iaxfriends_name_host` (`name`,`host`),
  KEY `iaxfriends_name_ipaddr_port` (`name`,`ipaddr`,`port`),
  KEY `iaxfriends_ipaddr_port` (`ipaddr`,`port`),
  KEY `iaxfriends_host_port` (`host`,`port`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `iaxfriends`
--

LOCK TABLES `iaxfriends` WRITE;
/*!40000 ALTER TABLE `iaxfriends` DISABLE KEYS */;
/*!40000 ALTER TABLE `iaxfriends` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `in_sms`
--

DROP TABLE IF EXISTS `in_sms`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `in_sms` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `date` datetime NOT NULL,
  `modem` varchar(10) COLLATE utf8_unicode_ci NOT NULL,
  `callerid` varchar(16) COLLATE utf8_unicode_ci NOT NULL,
  `msg` text COLLATE utf8_unicode_ci NOT NULL,
  `oktell` enum('0','1') COLLATE utf8_unicode_ci NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci COMMENT='Входящие SMS';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `in_sms`
--

LOCK TABLES `in_sms` WRITE;
/*!40000 ALTER TABLE `in_sms` DISABLE KEYS */;
/*!40000 ALTER TABLE `in_sms` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `invoice`
--

DROP TABLE IF EXISTS `invoice`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `invoice` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `num` varchar(10) NOT NULL,
  `created` datetime NOT NULL,
  `user` int(11) NOT NULL DEFAULT '0',
  `seller` int(11) NOT NULL,
  `customer` int(11) NOT NULL,
  `paid` datetime DEFAULT NULL,
  `tax` decimal(19,4) NOT NULL DEFAULT '0.0000',
  `summ` decimal(19,4) NOT NULL DEFAULT '0.0000',
  `notes` varchar(254) DEFAULT NULL,
  `total` decimal(19,4) NOT NULL DEFAULT '0.0000',
  `state` int(11) NOT NULL DEFAULT '0',
  `office` int(11) NOT NULL DEFAULT '0',
  `type` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`),
  KEY `FK_invoice_users` (`user`),
  KEY `FK_invoice_offices` (`office`),
  KEY `state` (`state`),
  KEY `FK_invoice_banks` (`seller`),
  KEY `FK_invoice_banks_2` (`customer`),
  KEY `num` (`num`),
  KEY `created` (`created`),
  CONSTRAINT `FK_invoice_banks` FOREIGN KEY (`seller`) REFERENCES `banks` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT `FK_invoice_banks_2` FOREIGN KEY (`customer`) REFERENCES `banks` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT `FK_invoice_offices` FOREIGN KEY (`office`) REFERENCES `offices` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT `FK_invoice_users` FOREIGN KEY (`user`) REFERENCES `users` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION
) ENGINE=InnoDB AUTO_INCREMENT=3 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `invoice`
--

LOCK TABLES `invoice` WRITE;
/*!40000 ALTER TABLE `invoice` DISABLE KEYS */;
INSERT INTO `invoice` VALUES (1,'1','2025-01-01 09:00:00',1,1,2,NULL,0.0000,1500.0000,NULL,1500.0000,2,1,1);
/*!40000 ALTER TABLE `invoice` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `invoice_items`
--

DROP TABLE IF EXISTS `invoice_items`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `invoice_items` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(254) NOT NULL,
  `price` decimal(19,4) NOT NULL DEFAULT '0.0000',
  `count` double NOT NULL DEFAULT '0',
  `units` varchar(50) NOT NULL COMMENT 'единицы измерения',
  `tax` double NOT NULL DEFAULT '0' COMMENT 'налог, проценты',
  `tax_mode` int(11) NOT NULL DEFAULT '0' COMMENT '0 NoTax, 1 Add, 2 Extract',
  `total` decimal(19,4) NOT NULL DEFAULT '0.0000' COMMENT 'сумма',
  `invoice` int(11) DEFAULT NULL COMMENT 'id счёта',
  `vat_invoice` int(11) DEFAULT NULL,
  `p_list` int(11) DEFAULT NULL,
  `w_list` int(11) DEFAULT NULL,
  `repair` int(11) DEFAULT NULL,
  `document` int(11) DEFAULT NULL,
  PRIMARY KEY (`id`),
  KEY `FK_invoice_items_invoice` (`invoice`),
  KEY `FK_invoice_items_workshop` (`repair`),
  KEY `FK_invoice_items_docs` (`document`),
  KEY `FK_invoice_items_vat_invoice` (`vat_invoice`),
  KEY `FK_invoice_items_p_lists` (`p_list`),
  KEY `FK_invoice_items_w_lists` (`w_list`),
  CONSTRAINT `FK_invoice_items_docs` FOREIGN KEY (`document`) REFERENCES `docs` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT `FK_invoice_items_invoice` FOREIGN KEY (`invoice`) REFERENCES `invoice` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT `FK_invoice_items_p_lists` FOREIGN KEY (`p_list`) REFERENCES `p_lists` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT `FK_invoice_items_vat_invoice` FOREIGN KEY (`vat_invoice`) REFERENCES `vat_invoice` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT `FK_invoice_items_w_lists` FOREIGN KEY (`w_list`) REFERENCES `w_lists` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT `FK_invoice_items_workshop` FOREIGN KEY (`repair`) REFERENCES `workshop` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `invoice_items`
--

LOCK TABLES `invoice_items` WRITE;
/*!40000 ALTER TABLE `invoice_items` DISABLE KEYS */;
/*!40000 ALTER TABLE `invoice_items` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `items_state`
--

DROP TABLE IF EXISTS `items_state`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `items_state` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(254) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `items_state`
--

LOCK TABLES `items_state` WRITE;
/*!40000 ALTER TABLE `items_state` DISABLE KEYS */;
/*!40000 ALTER TABLE `items_state` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `kkt`
--

DROP TABLE IF EXISTS `kkt`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `kkt` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(254) NOT NULL,
  `office` int(11) DEFAULT NULL,
  `protocol` int(11) NOT NULL DEFAULT '0',
  `ip` varchar(50) DEFAULT NULL,
  `port` int(11) DEFAULT NULL,
  `tax_type` int(11) NOT NULL DEFAULT '3' COMMENT 'Код Налогообложения',
  `tax` int(11) NOT NULL DEFAULT '0',
  `r_simple` tinyint(1) NOT NULL DEFAULT '1',
  `s_simple` tinyint(1) NOT NULL DEFAULT '1',
  `r_tpl` varchar(254) DEFAULT NULL,
  `s_tpl` varchar(254) DEFAULT NULL,
  `footer` varchar(254) DEFAULT NULL,
  `archive` tinyint(1) NOT NULL DEFAULT '0',
  `order_payment_item_sign` int(11) NOT NULL DEFAULT '1',
  `product_payment_item_sign` int(11) NOT NULL DEFAULT '1',
  `operator` varchar(50) DEFAULT NULL COMMENT 'Кассир',
  `operator_inn` varchar(50) DEFAULT NULL COMMENT 'ИНН',
  PRIMARY KEY (`id`),
  KEY `FK_kkt_offices` (`office`),
  CONSTRAINT `FK_kkt_offices` FOREIGN KEY (`office`) REFERENCES `offices` (`id`) ON DELETE SET NULL ON UPDATE NO ACTION
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `kkt`
--

LOCK TABLES `kkt` WRITE;
/*!40000 ALTER TABLE `kkt` DISABLE KEYS */;
/*!40000 ALTER TABLE `kkt` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `kkt_employee`
--

DROP TABLE IF EXISTS `kkt_employee`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `kkt_employee` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `kkt` int(11) NOT NULL,
  `employee` int(11) NOT NULL,
  `office` int(11) NOT NULL,
  PRIMARY KEY (`id`),
  KEY `FK_kkt_employee_kkt` (`kkt`),
  KEY `FK_kkt_employee_users` (`employee`),
  KEY `FK_kkt_employee_offices` (`office`),
  CONSTRAINT `FK_kkt_employee_kkt` FOREIGN KEY (`kkt`) REFERENCES `kkt` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT `FK_kkt_employee_offices` FOREIGN KEY (`office`) REFERENCES `offices` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT `FK_kkt_employee_users` FOREIGN KEY (`employee`) REFERENCES `users` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `kkt_employee`
--

LOCK TABLES `kkt_employee` WRITE;
/*!40000 ALTER TABLE `kkt_employee` DISABLE KEYS */;
/*!40000 ALTER TABLE `kkt_employee` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `logs`
--

DROP TABLE IF EXISTS `logs`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `logs` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `group` int(11) DEFAULT NULL COMMENT '1 = ACP\n2 = Clients\n3 = Workshop\n4 = User\n5 = Items',
  `type` int(11) DEFAULT NULL COMMENT '        /* groups\n         * 1 = ACP\n         * 2 = Clients\n         * 3 = Repair\n         * 4 = Users\n         * 5 = Documents\n         * 6 = Store_items\n7 = kassa\n         */',
  `arh` tinyint(1) DEFAULT '0' COMMENT '1 удалено',
  `user` int(11) NOT NULL,
  `created` datetime DEFAULT NULL,
  `values` text,
  `values_after` text,
  `office` int(11) NOT NULL,
  `client` int(11) DEFAULT NULL,
  `repair` int(11) DEFAULT NULL,
  `item` int(11) DEFAULT NULL,
  `document` int(11) DEFAULT NULL,
  `cash_order` int(11) DEFAULT NULL,
  `part_request` int(11) DEFAULT NULL,
  `notes` text,
  PRIMARY KEY (`id`),
  KEY `fk_logs_users1_idx` (`user`),
  KEY `fk_logs_offices1_idx` (`office`),
  KEY `fk_logs_clients1_idx` (`client`),
  KEY `fk_logs_workshop1_idx` (`repair`),
  KEY `FK_logs_store_items` (`item`),
  KEY `FK_logs_cash_orders` (`cash_order`),
  KEY `item` (`item`),
  KEY `document` (`document`),
  KEY `FK_logs_parts_request` (`part_request`),
  CONSTRAINT `FK_logs_cash_orders` FOREIGN KEY (`cash_order`) REFERENCES `cash_orders` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT `FK_logs_parts_request` FOREIGN KEY (`part_request`) REFERENCES `parts_request` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT `FK_logs_store_items` FOREIGN KEY (`item`) REFERENCES `store_items` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT `fk_logs_clients1` FOREIGN KEY (`client`) REFERENCES `clients` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT `fk_logs_offices1` FOREIGN KEY (`office`) REFERENCES `offices` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT `fk_logs_users1` FOREIGN KEY (`user`) REFERENCES `users` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT `fk_logs_workshop1` FOREIGN KEY (`repair`) REFERENCES `workshop` (`id`) ON DELETE CASCADE ON UPDATE NO ACTION
) ENGINE=InnoDB AUTO_INCREMENT=309 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `logs`
--

LOCK TABLES `logs` WRITE;
/*!40000 ALTER TABLE `logs` DISABLE KEYS */;
/*!40000 ALTER TABLE `logs` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `materials`
--

DROP TABLE IF EXISTS `materials`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `materials` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `type` int(11) NOT NULL,
  `count` int(11) NOT NULL DEFAULT '0',
  `articul` int(11) DEFAULT NULL,
  `name` varchar(254) NOT NULL,
  `card_id` int(11) NOT NULL,
  `price` decimal(19,4) NOT NULL DEFAULT '0.0000',
  `works_price` decimal(19,4) NOT NULL DEFAULT '0.0000',
  `salary_summ` decimal(19,4) DEFAULT NULL,
  PRIMARY KEY (`id`),
  KEY `type` (`type`),
  KEY `FK_materials_cartridge_cards` (`card_id`),
  CONSTRAINT `FK_materials_cartridge_cards` FOREIGN KEY (`card_id`) REFERENCES `cartridge_cards` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `materials`
--

LOCK TABLES `materials` WRITE;
/*!40000 ALTER TABLE `materials` DISABLE KEYS */;
/*!40000 ALTER TABLE `materials` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `media_info`
--

DROP TABLE IF EXISTS `media_info`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `media_info` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `state` int(11) NOT NULL DEFAULT '0' COMMENT '0 запись начата 1 запись окончена 2 идёт синхронизация 3 данные синхронинированы',
  `client_id` int(11) DEFAULT NULL,
  `repair_id` int(11) DEFAULT NULL,
  `sale_id` int(11) DEFAULT NULL,
  `record_start` datetime NOT NULL,
  `record_end` datetime DEFAULT NULL,
  `file` varchar(254) NOT NULL,
  `user_id` int(11) NOT NULL,
  PRIMARY KEY (`id`),
  KEY `fk_media_info_clients1_idx` (`client_id`),
  KEY `fk_media_info_workshop1_idx` (`repair_id`),
  KEY `fk_media_info_docs1_idx` (`sale_id`),
  KEY `fk_media_info_users1_idx` (`user_id`),
  CONSTRAINT `fk_media_info_clients1` FOREIGN KEY (`client_id`) REFERENCES `clients` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT `fk_media_info_docs1` FOREIGN KEY (`sale_id`) REFERENCES `docs` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT `fk_media_info_users1` FOREIGN KEY (`user_id`) REFERENCES `users` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT `fk_media_info_workshop1` FOREIGN KEY (`repair_id`) REFERENCES `workshop` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `media_info`
--

LOCK TABLES `media_info` WRITE;
/*!40000 ALTER TABLE `media_info` DISABLE KEYS */;
/*!40000 ALTER TABLE `media_info` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `meetme`
--

DROP TABLE IF EXISTS `meetme`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `meetme` (
  `bookid` int(11) NOT NULL AUTO_INCREMENT,
  `confno` varchar(80) NOT NULL,
  `starttime` datetime DEFAULT NULL,
  `endtime` datetime DEFAULT NULL,
  `pin` varchar(20) DEFAULT NULL,
  `adminpin` varchar(20) DEFAULT NULL,
  `opts` varchar(20) DEFAULT NULL,
  `adminopts` varchar(20) DEFAULT NULL,
  `recordingfilename` varchar(80) DEFAULT NULL,
  `recordingformat` varchar(10) DEFAULT NULL,
  `maxusers` int(11) DEFAULT NULL,
  `members` int(11) NOT NULL,
  PRIMARY KEY (`bookid`),
  KEY `meetme_confno_start_end` (`confno`,`starttime`,`endtime`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `meetme`
--

LOCK TABLES `meetme` WRITE;
/*!40000 ALTER TABLE `meetme` DISABLE KEYS */;
/*!40000 ALTER TABLE `meetme` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `modal`
--

DROP TABLE IF EXISTS `modal`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `modal` (
  `id` int(11) NOT NULL,
  `title` varchar(45) DEFAULT NULL,
  `text` text,
  `cookie` varchar(45) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `modal`
--

LOCK TABLES `modal` WRITE;
/*!40000 ALTER TABLE `modal` DISABLE KEYS */;
/*!40000 ALTER TABLE `modal` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `musiconhold`
--

DROP TABLE IF EXISTS `musiconhold`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `musiconhold` (
  `name` varchar(80) NOT NULL,
  `mode` enum('custom','files','mp3nb','quietmp3nb','quietmp3') DEFAULT NULL,
  `directory` varchar(255) DEFAULT NULL,
  `application` varchar(255) DEFAULT NULL,
  `digit` varchar(1) DEFAULT NULL,
  `sort` varchar(10) DEFAULT NULL,
  `format` varchar(10) DEFAULT NULL,
  `stamp` datetime DEFAULT NULL,
  PRIMARY KEY (`name`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `musiconhold`
--

LOCK TABLES `musiconhold` WRITE;
/*!40000 ALTER TABLE `musiconhold` DISABLE KEYS */;
/*!40000 ALTER TABLE `musiconhold` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `notifications`
--

DROP TABLE IF EXISTS `notifications`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `notifications` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `created` datetime NOT NULL,
  `employee` int(11) NOT NULL,
  `type` int(11) NOT NULL,
  `customer` int(11) DEFAULT NULL,
  `task_id` int(11) DEFAULT NULL,
  `repair_id` int(11) DEFAULT NULL,
  `sms_id` int(11) DEFAULT NULL,
  `request_id` int(11) DEFAULT NULL COMMENT 'Внутренние резервы',
  `part_request_id` int(11) DEFAULT NULL,
  `subject` varchar(254) NOT NULL,
  `text` varchar(1000) NOT NULL,
  `tel` varchar(45) DEFAULT NULL,
  `status` tinyint(1) NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`),
  KEY `FK_notifications_tasks` (`task_id`),
  KEY `FK_notifications_workshop` (`repair_id`),
  KEY `created` (`created`),
  KEY `FK_notifications_users` (`employee`),
  KEY `FK_notifications_in_sms` (`sms_id`),
  KEY `FK_notifications_store_int_reserve` (`request_id`),
  KEY `FK_notifications_parts_request` (`part_request_id`),
  KEY `FK_notifications_clients` (`customer`),
  CONSTRAINT `FK_notifications_clients` FOREIGN KEY (`customer`) REFERENCES `clients` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT `FK_notifications_in_sms` FOREIGN KEY (`sms_id`) REFERENCES `in_sms` (`id`) ON DELETE SET NULL,
  CONSTRAINT `FK_notifications_parts_request` FOREIGN KEY (`part_request_id`) REFERENCES `parts_request` (`id`) ON DELETE SET NULL,
  CONSTRAINT `FK_notifications_store_int_reserve` FOREIGN KEY (`request_id`) REFERENCES `store_int_reserve` (`id`) ON DELETE SET NULL,
  CONSTRAINT `FK_notifications_tasks` FOREIGN KEY (`task_id`) REFERENCES `tasks` (`idt`) ON DELETE SET NULL ON UPDATE NO ACTION,
  CONSTRAINT `FK_notifications_users` FOREIGN KEY (`employee`) REFERENCES `users` (`id`) ON DELETE CASCADE,
  CONSTRAINT `FK_notifications_workshop` FOREIGN KEY (`repair_id`) REFERENCES `workshop` (`id`) ON DELETE SET NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `notifications`
--

LOCK TABLES `notifications` WRITE;
/*!40000 ALTER TABLE `notifications` DISABLE KEYS */;
/*!40000 ALTER TABLE `notifications` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `offices`
--

DROP TABLE IF EXISTS `offices`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `offices` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `state` int(11) NOT NULL DEFAULT '1' COMMENT '0 архивный\n1 активный',
  `name` varchar(254) NOT NULL,
  `address` varchar(254) DEFAULT NULL,
  `phone` varchar(45) DEFAULT NULL,
  `logo` mediumblob,
  `administrator` int(11) NOT NULL,
  `created` datetime DEFAULT NULL,
  `phone2` varchar(45) DEFAULT NULL,
  `default_company` int(11) NOT NULL,
  `card_payment` tinyint(1) NOT NULL DEFAULT '0',
  `use_boxes` tinyint(1) NOT NULL DEFAULT '1',
  `paint_repairs` tinyint(1) NOT NULL DEFAULT '1',
  `warranty_sn` tinyint(1) NOT NULL DEFAULT '1',
  PRIMARY KEY (`id`),
  KEY `fk_offices_users1_idx` (`administrator`),
  KEY `fk_offices_companies1_idx` (`default_company`),
  CONSTRAINT `fk_offices_companies1` FOREIGN KEY (`default_company`) REFERENCES `companies` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT `fk_offices_users1` FOREIGN KEY (`administrator`) REFERENCES `users` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `offices`
--

LOCK TABLES `offices` WRITE;
/*!40000 ALTER TABLE `offices` DISABLE KEYS */;
INSERT INTO `offices` VALUES (1,1,'Центральный','ул. Менделеева, 16','+7 123 456 78 90',NULL,1,'2000-01-01 09:00:00','',1,0,1,1,1);
/*!40000 ALTER TABLE `offices` ENABLE KEYS */;
UNLOCK TABLES;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8mb4 */ ;
/*!50003 SET character_set_results = utf8mb4 */ ;
/*!50003 SET collation_connection  = utf8mb4_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50017 DEFINER=`root`@`%`*/ /*!50003 TRIGGER `offices_BEFORE_INSERT` BEFORE INSERT ON `offices` FOR EACH ROW BEGIN
SET NEW.created = NOW();
END */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;

--
-- Table structure for table `order_items`
--

DROP TABLE IF EXISTS `order_items`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `order_items` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `order_id` int(11) NOT NULL,
  `item_id` int(11) NOT NULL,
  `item_count` int(11) NOT NULL DEFAULT '1',
  PRIMARY KEY (`id`),
  KEY `fk_order_items_cash_orders1_idx` (`order_id`),
  KEY `fk_order_items_store_items1_idx` (`item_id`),
  CONSTRAINT `fk_order_items_cash_orders1` FOREIGN KEY (`order_id`) REFERENCES `cash_orders` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT `fk_order_items_store_items1` FOREIGN KEY (`item_id`) REFERENCES `store_items` (`id`) ON DELETE CASCADE ON UPDATE NO ACTION
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `order_items`
--

LOCK TABLES `order_items` WRITE;
/*!40000 ALTER TABLE `order_items` DISABLE KEYS */;
/*!40000 ALTER TABLE `order_items` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `out_sms`
--

DROP TABLE IF EXISTS `out_sms`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `out_sms` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `state` int(11) NOT NULL DEFAULT '0',
  `created` datetime NOT NULL,
  `user` int(11) NOT NULL,
  `client` int(11) DEFAULT NULL,
  `msg` varchar(254) NOT NULL DEFAULT '',
  `phone` varchar(40) NOT NULL,
  `status_code` int(11) DEFAULT NULL COMMENT 'Код статуса',
  `service` int(11) NOT NULL DEFAULT '1' COMMENT 'Сервис отправки',
  `sms_id` varchar(50) NOT NULL DEFAULT '0' COMMENT 'Идентификатор сообщения',
  PRIMARY KEY (`id`),
  KEY `FK_out_sms_users` (`user`),
  KEY `FK_out_sms_clients` (`client`),
  KEY `created` (`created`),
  CONSTRAINT `FK_out_sms_clients` FOREIGN KEY (`client`) REFERENCES `clients` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT `FK_out_sms_users` FOREIGN KEY (`user`) REFERENCES `users` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `out_sms`
--

LOCK TABLES `out_sms` WRITE;
/*!40000 ALTER TABLE `out_sms` DISABLE KEYS */;
/*!40000 ALTER TABLE `out_sms` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `p_lists`
--

DROP TABLE IF EXISTS `p_lists`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `p_lists` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `num` varchar(10) NOT NULL,
  `created` datetime NOT NULL,
  `invoice` int(11) DEFAULT NULL,
  `user` int(11) NOT NULL,
  `seller` int(11) NOT NULL,
  `customer` int(11) NOT NULL,
  `office` int(11) NOT NULL,
  `tax` decimal(19,4) NOT NULL DEFAULT '0.0000',
  `summ` decimal(19,4) NOT NULL DEFAULT '0.0000',
  `total` decimal(19,4) NOT NULL DEFAULT '0.0000',
  `notes` varchar(254) DEFAULT NULL,
  `reason` varchar(254) DEFAULT NULL,
  PRIMARY KEY (`id`),
  KEY `FK_p_lists_invoice` (`invoice`),
  KEY `FK_p_lists_users` (`user`),
  KEY `FK_p_lists_offices` (`office`),
  KEY `num` (`num`),
  KEY `created` (`created`),
  KEY `FK_p_lists_seller` (`seller`),
  KEY `FK_p_lists_customer` (`customer`),
  CONSTRAINT `FK_p_lists_customer` FOREIGN KEY (`customer`) REFERENCES `banks` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT `FK_p_lists_invoice` FOREIGN KEY (`invoice`) REFERENCES `invoice` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT `FK_p_lists_offices` FOREIGN KEY (`office`) REFERENCES `offices` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT `FK_p_lists_seller` FOREIGN KEY (`seller`) REFERENCES `banks` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT `FK_p_lists_users` FOREIGN KEY (`user`) REFERENCES `users` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Товарные накладные';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `p_lists`
--

LOCK TABLES `p_lists` WRITE;
/*!40000 ALTER TABLE `p_lists` DISABLE KEYS */;
/*!40000 ALTER TABLE `p_lists` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `parts_request`
--

DROP TABLE IF EXISTS `parts_request`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `parts_request` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `request_time` datetime NOT NULL,
  `from_user` int(11) NOT NULL,
  `repair` int(11) DEFAULT NULL,
  `client` int(11) DEFAULT NULL,
  `state` int(11) NOT NULL COMMENT '1 - не начато;\r\n2 - выполнено;\r\n3 - отложено;\r\n4 - в процессе;\r\n5 - отменено',
  `end_date` datetime DEFAULT NULL,
  `plan_end_date` datetime DEFAULT NULL,
  `summ` decimal(19,4) DEFAULT NULL,
  `tracking` varchar(254) DEFAULT NULL,
  `item_id` int(11) DEFAULT NULL,
  `item_name` varchar(254) NOT NULL,
  `notes` text,
  `url` varchar(254) NOT NULL,
  `pririty` int(11) NOT NULL,
  `count` int(11) NOT NULL DEFAULT '1',
  `dealer` int(11) DEFAULT NULL,
  PRIMARY KEY (`id`),
  KEY `FK_parts_request_users` (`from_user`),
  KEY `FK_parts_request_workshop` (`repair`),
  KEY `FK_parts_request_clients` (`client`),
  KEY `FK_parts_request_store_items` (`item_id`),
  KEY `FK_parts_request_clients_2` (`dealer`),
  CONSTRAINT `FK_parts_request_clients` FOREIGN KEY (`client`) REFERENCES `clients` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT `FK_parts_request_clients_2` FOREIGN KEY (`dealer`) REFERENCES `clients` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT `FK_parts_request_store_items` FOREIGN KEY (`item_id`) REFERENCES `store_items` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT `FK_parts_request_users` FOREIGN KEY (`from_user`) REFERENCES `users` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT `FK_parts_request_workshop` FOREIGN KEY (`repair`) REFERENCES `workshop` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION
) ENGINE=InnoDB AUTO_INCREMENT=3 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `parts_request`
--

LOCK TABLES `parts_request` WRITE;
/*!40000 ALTER TABLE `parts_request` DISABLE KEYS */;
INSERT INTO `parts_request` VALUES (1,'2025-01-01 09:00:00',7,1,864,1,NULL,NULL,NULL,NULL,NULL,'Клавиатура для ноутбука Acer E5-522 / E5-573 (черный)',NULL,'',1,2,1952);
/*!40000 ALTER TABLE `parts_request` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `parts_request_employees`
--

DROP TABLE IF EXISTS `parts_request_employees`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `parts_request_employees` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `emploee` int(11) NOT NULL DEFAULT '0',
  `request` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`),
  KEY `FK_parts_request_employees_users` (`emploee`),
  KEY `FK_parts_request_employees_parts_request` (`request`),
  CONSTRAINT `FK_parts_request_employees_parts_request` FOREIGN KEY (`request`) REFERENCES `parts_request` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT `FK_parts_request_employees_users` FOREIGN KEY (`emploee`) REFERENCES `users` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION
) ENGINE=InnoDB AUTO_INCREMENT=4 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `parts_request_employees`
--

LOCK TABLES `parts_request_employees` WRITE;
/*!40000 ALTER TABLE `parts_request_employees` DISABLE KEYS */;
/*!40000 ALTER TABLE `parts_request_employees` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `parts_request_suppliers`
--

DROP TABLE IF EXISTS `parts_request_suppliers`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `parts_request_suppliers` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `request_id` int(11) NOT NULL,
  `supplier_id` int(11) DEFAULT NULL,
  `supplier_url` text,
  `item_url` text NOT NULL,
  `moq` int(11) unsigned NOT NULL DEFAULT '1' COMMENT 'Minimum order quantity',
  `price` decimal(11,4) unsigned NOT NULL DEFAULT '0.0000' COMMENT 'Price per piece',
  `select` tinyint(3) unsigned NOT NULL DEFAULT '0' COMMENT 'Final choice sign',
  `notes` varchar(254) DEFAULT NULL,
  PRIMARY KEY (`id`),
  KEY `FK_parts_requests_suppliers_clients` (`supplier_id`),
  KEY `FK_parts_requests_suppliers_parts_request` (`request_id`),
  CONSTRAINT `FK_parts_requests_suppliers_clients` FOREIGN KEY (`supplier_id`) REFERENCES `clients` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT `FK_parts_requests_suppliers_parts_request` FOREIGN KEY (`request_id`) REFERENCES `parts_request` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `parts_request_suppliers`
--

LOCK TABLES `parts_request_suppliers` WRITE;
/*!40000 ALTER TABLE `parts_request_suppliers` DISABLE KEYS */;
/*!40000 ALTER TABLE `parts_request_suppliers` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `payment_systems`
--

DROP TABLE IF EXISTS `payment_systems`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `payment_systems` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `system_id` int(11) NOT NULL DEFAULT '0',
  `name` varchar(254) NOT NULL DEFAULT '0',
  `system_data` varchar(254) DEFAULT '0',
  `is_enable` tinyint(1) NOT NULL DEFAULT '1',
  PRIMARY KEY (`id`),
  UNIQUE KEY `system_id_unique` (`system_id`),
  KEY `system_id` (`system_id`)
) ENGINE=InnoDB AUTO_INCREMENT=5 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `payment_systems`
--

LOCK TABLES `payment_systems` WRITE;
/*!40000 ALTER TABLE `payment_systems` DISABLE KEYS */;
INSERT INTO `payment_systems` VALUES (1,-2,'Баланс клиента','',1),(2,-1,'Картой','0',0),(3,0,'Наличные','0',1),(4,1,'Безнал','0',1);
/*!40000 ALTER TABLE `payment_systems` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `payment_type_users`
--

DROP TABLE IF EXISTS `payment_type_users`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `payment_type_users` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `payment_type` int(11) NOT NULL DEFAULT '0',
  `user_id` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`),
  KEY `FK_payment_type_users_payment_types` (`payment_type`),
  KEY `FK_payment_type_users_users` (`user_id`),
  CONSTRAINT `FK_payment_type_users_payment_types` FOREIGN KEY (`payment_type`) REFERENCES `payment_types` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT `FK_payment_type_users_users` FOREIGN KEY (`user_id`) REFERENCES `users` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `payment_type_users`
--

LOCK TABLES `payment_type_users` WRITE;
/*!40000 ALTER TABLE `payment_type_users` DISABLE KEYS */;
/*!40000 ALTER TABLE `payment_type_users` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `payment_types`
--

DROP TABLE IF EXISTS `payment_types`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `payment_types` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `type` int(11) NOT NULL DEFAULT '0',
  `name` varchar(254) NOT NULL DEFAULT '0',
  `client` int(11) DEFAULT '0',
  `periodic` tinyint(1) NOT NULL DEFAULT '1',
  `pay_date` datetime DEFAULT NULL,
  `def_summ` decimal(19,4) DEFAULT NULL,
  `reason` text,
  `is_archive` tinyint(1) NOT NULL DEFAULT '0',
  `updated_at` datetime DEFAULT NULL,
  `payment_system` int(11) DEFAULT NULL,
  PRIMARY KEY (`id`),
  KEY `FK_payment_types_clients` (`client`),
  KEY `FK_payment_types_payment_systems` (`payment_system`),
  CONSTRAINT `FK_payment_types_clients` FOREIGN KEY (`client`) REFERENCES `clients` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT `FK_payment_types_payment_systems` FOREIGN KEY (`payment_system`) REFERENCES `payment_systems` (`system_id`) ON DELETE SET NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `payment_types`
--

LOCK TABLES `payment_types` WRITE;
/*!40000 ALTER TABLE `payment_types` DISABLE KEYS */;
/*!40000 ALTER TABLE `payment_types` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `permissions`
--

DROP TABLE IF EXISTS `permissions`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `permissions` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(254) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=109 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `permissions`
--

LOCK TABLES `permissions` WRITE;
/*!40000 ALTER TABLE `permissions` DISABLE KEYS */;
INSERT INTO `permissions` VALUES (1,'Изменять служебные настройки'),(2,'Принимать устройства в ремонт'),(3,'Печатать ценники и стикеры'),(4,'Выдавать устройство из ремонта'),(5,'Быстрая продажа'),(6,'Заносить товар на склад (Приход товара)'),(7,'Видеть список клиентов и их данные'),(8,'Создавать новых  клиентов'),(9,'Списывать товар со склада'),(10,'Редактировать цены на товары'),(11,'Редактировать существующих клиентов'),(12,'Генерировать и смотреть отчёты'),(15,'Просмотр финансовых документов'),(16,'Создавать ПКО/РКО'),(18,'Создавать Z отчёт (выемка средств с кассы)'),(20,'Просмотр кассы'),(21,'Редактировать товары (наименование ячейки, etc)'),(22,'Читать SMS'),(23,'Открывать карточки ремонта других пользователей'),(24,'Использовать телефонию'),(25,'Администрирование карточки ремонта '),(26,'Статус: Приём в ремонт'),(27,'Статус: Проведение диагностики'),(28,'Статус: Ожидание решения клиента'),(29,'Статус: Согласовано с клиентом'),(30,'Статус: Выполнение работ'),(31,'Статус: Ожидание комплектующих'),(32,'Статус: Готово к выдаче'),(33,'Статус: Готово к выдаче без ремонта'),(34,'Статус: Выдано клиенту'),(35,'Статус: Неисправность не проявилась'),(36,'Статус: Смена офиса'),(37,'Статус: Клиент отказался от ремонта'),(38,'Редактировать прайс лист услуг'),(39,'Редактировать каталог товаров'),(40,'Выгружать товары в интернет магазины'),(41,'Видеть цены для СЦ'),(42,'Видеть розничные цены'),(43,'Видеть оптовые цены'),(44,'Брать товары со склада'),(45,'Выдавать товары сотрудникам'),(46,'Управление складом'),(47,'Исключать товар из не выданного ремонта'),(48,'Выдавать аванс сотрудникам'),(49,'Удаленный вход'),(50,'Добавлять работы не из прайс листа'),(51,'Распроводить расходные накладные'),(52,'Делать дополнительные начисления сотрудникам'),(53,'Статус: Подтверждение диагностики'),(54,'Открывать кассу чужого офиса'),(55,'Менять статус информирования клиента'),(56,'Распроводить приходные накладные'),(57,'Мастер может брать свободный ремонт'),(58,'Принимать заявки из интернет магазинов'),(59,'Менять офис при входе'),(60,'Назначать ответственного инженера'),(61,'Менять цвет ремонта'),(62,'Включать видео запись если она не активна'),(63,'Отправлять SMS'),(64,'Статус: Выдано в долг'),(65,'Работать с безналичными счетами'),(66,'Создавать резервные копии БД'),(67,'Создавать задания'),(68,'Видеть задания других пользователей'),(69,'Устанавливать детали со склада'),(70,'Использовать менеджер закупок'),(71,'Проводить документы задним числом'),(72,'Изменять офис ремонта'),(73,'Проводить быстрый ремонт'),(74,'Редактировать ремонты другого офиса'),(75,'Работать с ККТ'),(76,'Назначать ответственного менеджера'),(77,'Видеть работы/детали других сотрудников'),(78,'Видеть историю в карте ремонта'),(79,'Изменять параметры ККТ'),(80,'Редактировать карты картриджей'),(81,'Отправлять Email'),(101,'editRepairIncomingSet'),(102,'editAnyComment'),(103,'advEditWorkList'),(104,'viewClientPassportData'),(105,'viewClientBankData'),(106,'editPaymentSystemInCommittedCashRegisters'),(107,'viewEmployeesSalary'),(108,'workWithTechReports');
/*!40000 ALTER TABLE `permissions` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `permissions_roles`
--

DROP TABLE IF EXISTS `permissions_roles`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `permissions_roles` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `permission_id` int(11) NOT NULL,
  `role_id` int(11) NOT NULL,
  PRIMARY KEY (`id`),
  KEY `fk_permissions_roles_permissions1_idx` (`permission_id`),
  KEY `fk_permissions_roles_roles1_idx` (`role_id`),
  CONSTRAINT `fk_permissions_roles_permissions1` FOREIGN KEY (`permission_id`) REFERENCES `permissions` (`id`) ON DELETE CASCADE,
  CONSTRAINT `fk_permissions_roles_roles1` FOREIGN KEY (`role_id`) REFERENCES `roles` (`id`) ON DELETE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=91 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `permissions_roles`
--

LOCK TABLES `permissions_roles` WRITE;
/*!40000 ALTER TABLE `permissions_roles` DISABLE KEYS */;
INSERT INTO `permissions_roles` VALUES (1,1,1),(2,2,1),(3,3,1),(4,4,1),(5,5,1),(6,6,1),(7,7,1),(8,8,1),(9,9,1),(10,10,1),(11,11,1),(12,12,1),(13,15,1),(14,16,1),(15,18,1),(16,20,1),(17,21,1),(18,22,1),(19,23,1),(20,24,1),(21,25,1),(22,26,1),(23,27,1),(24,28,1),(25,29,1),(26,30,1),(27,31,1),(28,32,1),(29,33,1),(30,34,1),(31,35,1),(32,36,1),(33,37,1),(34,38,1),(35,39,1),(36,40,1),(37,41,1),(38,42,1),(39,43,1),(40,44,1),(41,45,1),(42,46,1),(43,47,1),(44,48,1),(45,49,1),(46,50,1),(47,51,1),(48,52,1),(49,53,1),(50,54,1),(51,55,1),(52,56,1),(53,57,1),(54,58,1),(55,59,1),(56,60,1),(57,61,1),(58,62,1),(59,63,1),(60,64,1),(61,65,1),(62,66,1),(63,67,1),(64,68,1),(65,69,1),(66,70,1),(67,71,1),(68,72,1),(69,73,1),(70,74,1),(71,75,1),(72,76,1),(73,77,1),(74,78,1),(75,79,1),(76,80,1),(77,81,1),(78,101,1),(79,102,1),(80,103,1),(81,104,1),(82,105,1),(83,106,1),(84,107,1),(85,108,1),(86,70,3),(87,70,6),(88,70,5),(90,70,2);
/*!40000 ALTER TABLE `permissions_roles` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `pinpad`
--

DROP TABLE IF EXISTS `pinpad`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `pinpad` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(50) NOT NULL,
  `office` int(11) NOT NULL,
  `kkt` int(11) DEFAULT NULL,
  `printer` int(11) DEFAULT NULL,
  `fee` double NOT NULL DEFAULT '0' COMMENT 'Банковский сбор',
  `fee_mode` tinyint(1) NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`),
  KEY `FK_pinpad_offices` (`office`),
  CONSTRAINT `FK_pinpad_offices` FOREIGN KEY (`office`) REFERENCES `offices` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `pinpad`
--

LOCK TABLES `pinpad` WRITE;
/*!40000 ALTER TABLE `pinpad` DISABLE KEYS */;
/*!40000 ALTER TABLE `pinpad` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `pinpad_logs`
--

DROP TABLE IF EXISTS `pinpad_logs`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `pinpad_logs` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `pinpad` int(11) NOT NULL,
  `created` datetime NOT NULL,
  `amount` int(11) NOT NULL DEFAULT '0',
  `TermNum` varchar(50) DEFAULT NULL,
  `ClientCard` varchar(20) DEFAULT NULL,
  `ClientExpiryDate` varchar(6) DEFAULT NULL,
  `AuthCode` varchar(50) DEFAULT NULL,
  `CardName` varchar(50) DEFAULT NULL,
  PRIMARY KEY (`id`),
  KEY `FK_pinpad_logs_pinpad` (`pinpad`),
  KEY `created` (`created`),
  CONSTRAINT `FK_pinpad_logs_pinpad` FOREIGN KEY (`pinpad`) REFERENCES `pinpad` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `pinpad_logs`
--

LOCK TABLES `pinpad_logs` WRITE;
/*!40000 ALTER TABLE `pinpad_logs` DISABLE KEYS */;
/*!40000 ALTER TABLE `pinpad_logs` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `ps_aors`
--

DROP TABLE IF EXISTS `ps_aors`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `ps_aors` (
  `id` varchar(40) NOT NULL,
  `contact` varchar(255) DEFAULT NULL,
  `default_expiration` int(11) DEFAULT NULL,
  `mailboxes` varchar(80) DEFAULT NULL,
  `max_contacts` int(11) DEFAULT NULL,
  `minimum_expiration` int(11) DEFAULT NULL,
  `remove_existing` enum('yes','no') DEFAULT NULL,
  `qualify_frequency` int(11) DEFAULT NULL,
  `authenticate_qualify` enum('yes','no') DEFAULT NULL,
  `maximum_expiration` int(11) DEFAULT NULL,
  `outbound_proxy` varchar(40) DEFAULT NULL,
  `support_path` enum('yes','no') DEFAULT NULL,
  `qualify_timeout` float DEFAULT NULL,
  `voicemail_extension` varchar(40) DEFAULT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `id` (`id`),
  KEY `ps_aors_id` (`id`),
  KEY `ps_aors_qualifyfreq_contact` (`qualify_frequency`,`contact`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `ps_aors`
--

LOCK TABLES `ps_aors` WRITE;
/*!40000 ALTER TABLE `ps_aors` DISABLE KEYS */;
/*!40000 ALTER TABLE `ps_aors` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `ps_auths`
--

DROP TABLE IF EXISTS `ps_auths`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `ps_auths` (
  `id` int(11) NOT NULL,
  `auth_type` enum('md5','userpass') DEFAULT NULL,
  `nonce_lifetime` int(11) DEFAULT NULL,
  `md5_cred` varchar(40) DEFAULT NULL,
  `password` varchar(80) DEFAULT NULL,
  `realm` varchar(40) DEFAULT NULL,
  `username` varchar(40) DEFAULT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `id` (`id`),
  KEY `ps_auths_id` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `ps_auths`
--

LOCK TABLES `ps_auths` WRITE;
/*!40000 ALTER TABLE `ps_auths` DISABLE KEYS */;
/*!40000 ALTER TABLE `ps_auths` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `ps_contacts`
--

DROP TABLE IF EXISTS `ps_contacts`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `ps_contacts` (
  `id` varchar(255) DEFAULT NULL,
  `uri` varchar(255) DEFAULT NULL,
  `expiration_time` bigint(20) DEFAULT NULL,
  `qualify_frequency` int(11) DEFAULT NULL,
  `outbound_proxy` varchar(40) DEFAULT NULL,
  `path` text,
  `user_agent` varchar(255) DEFAULT NULL,
  `qualify_timeout` float DEFAULT NULL,
  `reg_server` varchar(20) DEFAULT NULL,
  `authenticate_qualify` enum('yes','no') DEFAULT NULL,
  `via_addr` varchar(40) DEFAULT NULL,
  `via_port` int(11) DEFAULT NULL,
  `call_id` varchar(255) DEFAULT NULL,
  `endpoint` varchar(40) DEFAULT NULL,
  UNIQUE KEY `id` (`id`),
  UNIQUE KEY `ps_contacts_uq` (`id`,`reg_server`),
  KEY `ps_contacts_id` (`id`),
  KEY `ps_contacts_qualifyfreq_exp` (`qualify_frequency`,`expiration_time`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `ps_contacts`
--

LOCK TABLES `ps_contacts` WRITE;
/*!40000 ALTER TABLE `ps_contacts` DISABLE KEYS */;
/*!40000 ALTER TABLE `ps_contacts` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `ps_domain_aliases`
--

DROP TABLE IF EXISTS `ps_domain_aliases`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `ps_domain_aliases` (
  `id` varchar(40) NOT NULL,
  `key` int(11) NOT NULL AUTO_INCREMENT,
  `domain` varchar(80) DEFAULT NULL,
  PRIMARY KEY (`key`),
  UNIQUE KEY `id` (`id`),
  KEY `ps_domain_aliases_id` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `ps_domain_aliases`
--

LOCK TABLES `ps_domain_aliases` WRITE;
/*!40000 ALTER TABLE `ps_domain_aliases` DISABLE KEYS */;
/*!40000 ALTER TABLE `ps_domain_aliases` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `ps_endpoint_id_ips`
--

DROP TABLE IF EXISTS `ps_endpoint_id_ips`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `ps_endpoint_id_ips` (
  `id` varchar(40) NOT NULL,
  `key` int(11) NOT NULL AUTO_INCREMENT,
  `endpoint` varchar(40) DEFAULT NULL,
  `match` varchar(80) DEFAULT NULL,
  PRIMARY KEY (`key`),
  UNIQUE KEY `id` (`id`),
  KEY `ps_endpoint_id_ips_id` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `ps_endpoint_id_ips`
--

LOCK TABLES `ps_endpoint_id_ips` WRITE;
/*!40000 ALTER TABLE `ps_endpoint_id_ips` DISABLE KEYS */;
/*!40000 ALTER TABLE `ps_endpoint_id_ips` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `ps_endpoints`
--

DROP TABLE IF EXISTS `ps_endpoints`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `ps_endpoints` (
  `id` int(11) NOT NULL,
  `transport` varchar(40) DEFAULT NULL,
  `aors` varchar(200) DEFAULT NULL,
  `auth` varchar(40) DEFAULT NULL,
  `context` varchar(40) DEFAULT NULL,
  `disallow` varchar(200) DEFAULT NULL,
  `allow` varchar(200) DEFAULT NULL,
  `direct_media` enum('yes','no') DEFAULT NULL,
  `connected_line_method` enum('invite','reinvite','update') DEFAULT NULL,
  `direct_media_method` enum('invite','reinvite','update') DEFAULT NULL,
  `direct_media_glare_mitigation` enum('none','outgoing','incoming') DEFAULT NULL,
  `disable_direct_media_on_nat` enum('yes','no') DEFAULT NULL,
  `dtmf_mode` enum('rfc4733','inband','info','auto') DEFAULT NULL,
  `external_media_address` varchar(40) DEFAULT NULL,
  `force_rport` enum('yes','no') DEFAULT NULL,
  `ice_support` enum('yes','no') DEFAULT NULL,
  `identify_by` enum('username','auth_username') DEFAULT NULL,
  `mailboxes` varchar(40) DEFAULT NULL,
  `moh_suggest` varchar(40) DEFAULT NULL,
  `outbound_auth` varchar(40) DEFAULT NULL,
  `outbound_proxy` varchar(40) DEFAULT NULL,
  `rewrite_contact` enum('yes','no') DEFAULT NULL,
  `rtp_ipv6` enum('yes','no') DEFAULT NULL,
  `rtp_symmetric` enum('yes','no') DEFAULT NULL,
  `send_diversion` enum('yes','no') DEFAULT NULL,
  `send_pai` enum('yes','no') DEFAULT NULL,
  `send_rpid` enum('yes','no') DEFAULT NULL,
  `timers_min_se` int(11) DEFAULT NULL,
  `timers` enum('forced','no','required','yes') DEFAULT NULL,
  `timers_sess_expires` int(11) DEFAULT NULL,
  `callerid` varchar(40) DEFAULT NULL,
  `callerid_privacy` enum('allowed_not_screened','allowed_passed_screened','allowed_failed_screened','allowed','prohib_not_screened','prohib_passed_screened','prohib_failed_screened','prohib','unavailable') DEFAULT NULL,
  `callerid_tag` varchar(40) DEFAULT NULL,
  `100rel` enum('no','required','yes') DEFAULT NULL,
  `aggregate_mwi` enum('yes','no') DEFAULT NULL,
  `trust_id_inbound` enum('yes','no') DEFAULT NULL,
  `trust_id_outbound` enum('yes','no') DEFAULT NULL,
  `use_ptime` enum('yes','no') DEFAULT NULL,
  `use_avpf` enum('yes','no') DEFAULT NULL,
  `media_encryption` enum('no','sdes','dtls') DEFAULT NULL,
  `inband_progress` enum('yes','no') DEFAULT NULL,
  `call_group` varchar(40) DEFAULT NULL,
  `pickup_group` varchar(40) DEFAULT NULL,
  `named_call_group` varchar(40) DEFAULT NULL,
  `named_pickup_group` varchar(40) DEFAULT NULL,
  `device_state_busy_at` int(11) DEFAULT NULL,
  `fax_detect` enum('yes','no') DEFAULT NULL,
  `t38_udptl` enum('yes','no') DEFAULT NULL,
  `t38_udptl_ec` enum('none','fec','redundancy') DEFAULT NULL,
  `t38_udptl_maxdatagram` int(11) DEFAULT NULL,
  `t38_udptl_nat` enum('yes','no') DEFAULT NULL,
  `t38_udptl_ipv6` enum('yes','no') DEFAULT NULL,
  `tone_zone` varchar(40) DEFAULT NULL,
  `language` varchar(40) DEFAULT NULL,
  `one_touch_recording` enum('yes','no') DEFAULT NULL,
  `record_on_feature` varchar(40) DEFAULT NULL,
  `record_off_feature` varchar(40) DEFAULT NULL,
  `rtp_engine` varchar(40) DEFAULT NULL,
  `allow_transfer` enum('yes','no') DEFAULT NULL,
  `allow_subscribe` enum('yes','no') DEFAULT NULL,
  `sdp_owner` varchar(40) DEFAULT NULL,
  `sdp_session` varchar(40) DEFAULT NULL,
  `tos_audio` varchar(10) DEFAULT NULL,
  `tos_video` varchar(10) DEFAULT NULL,
  `sub_min_expiry` int(11) DEFAULT NULL,
  `from_domain` varchar(40) DEFAULT NULL,
  `from_user` varchar(40) DEFAULT NULL,
  `mwi_from_user` varchar(40) DEFAULT NULL,
  `dtls_verify` varchar(40) DEFAULT NULL,
  `dtls_rekey` varchar(40) DEFAULT NULL,
  `dtls_cert_file` varchar(200) DEFAULT NULL,
  `dtls_private_key` varchar(200) DEFAULT NULL,
  `dtls_cipher` varchar(200) DEFAULT NULL,
  `dtls_ca_file` varchar(200) DEFAULT NULL,
  `dtls_ca_path` varchar(200) DEFAULT NULL,
  `dtls_setup` enum('active','passive','actpass') DEFAULT NULL,
  `srtp_tag_32` enum('yes','no') DEFAULT NULL,
  `media_address` varchar(40) DEFAULT NULL,
  `redirect_method` enum('user','uri_core','uri_pjsip') DEFAULT NULL,
  `set_var` text,
  `cos_audio` int(11) DEFAULT NULL,
  `cos_video` int(11) DEFAULT NULL,
  `message_context` varchar(40) DEFAULT NULL,
  `force_avp` enum('yes','no') DEFAULT NULL,
  `media_use_received_transport` enum('yes','no') DEFAULT NULL,
  `accountcode` varchar(80) DEFAULT NULL,
  `user_eq_phone` enum('yes','no') DEFAULT NULL,
  `moh_passthrough` enum('yes','no') DEFAULT NULL,
  `media_encryption_optimistic` enum('yes','no') DEFAULT NULL,
  `rpid_immediate` enum('yes','no') DEFAULT NULL,
  `g726_non_standard` enum('yes','no') DEFAULT NULL,
  `rtp_keepalive` int(11) DEFAULT NULL,
  `rtp_timeout` int(11) DEFAULT NULL,
  `rtp_timeout_hold` int(11) DEFAULT NULL,
  `bind_rtp_to_media_address` enum('yes','no') DEFAULT NULL,
  `voicemail_extension` varchar(40) DEFAULT NULL,
  `mwi_subscribe_replaces_unsolicited` int(11) DEFAULT NULL,
  `deny` varchar(95) DEFAULT NULL,
  `permit` varchar(95) DEFAULT NULL,
  `acl` varchar(40) DEFAULT NULL,
  `contact_deny` varchar(95) DEFAULT NULL,
  `contact_permit` varchar(95) DEFAULT NULL,
  `contact_acl` varchar(40) DEFAULT NULL,
  `subscribe_context` varchar(40) DEFAULT NULL,
  `fax_detect_timeout` int(11) DEFAULT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `id` (`id`),
  KEY `ps_endpoints_id` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `ps_endpoints`
--

LOCK TABLES `ps_endpoints` WRITE;
/*!40000 ALTER TABLE `ps_endpoints` DISABLE KEYS */;
/*!40000 ALTER TABLE `ps_endpoints` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `ps_globals`
--

DROP TABLE IF EXISTS `ps_globals`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `ps_globals` (
  `id` varchar(40) NOT NULL,
  `max_forwards` int(11) DEFAULT NULL,
  `user_agent` varchar(255) DEFAULT NULL,
  `default_outbound_endpoint` varchar(40) DEFAULT NULL,
  `debug` varchar(40) DEFAULT NULL,
  `endpoint_identifier_order` varchar(40) DEFAULT NULL,
  `max_initial_qualify_time` int(11) DEFAULT NULL,
  `default_from_user` varchar(80) DEFAULT NULL,
  `keep_alive_interval` int(11) DEFAULT NULL,
  `regcontext` varchar(80) DEFAULT NULL,
  `contact_expiration_check_interval` int(11) DEFAULT NULL,
  `default_voicemail_extension` varchar(40) DEFAULT NULL,
  `disable_multi_domain` enum('yes','no') DEFAULT NULL,
  `unidentified_request_count` int(11) DEFAULT NULL,
  `unidentified_request_period` int(11) DEFAULT NULL,
  `unidentified_request_prune_interval` int(11) DEFAULT NULL,
  `default_realm` varchar(40) DEFAULT NULL,
  UNIQUE KEY `id` (`id`),
  KEY `ps_globals_id` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `ps_globals`
--

LOCK TABLES `ps_globals` WRITE;
/*!40000 ALTER TABLE `ps_globals` DISABLE KEYS */;
/*!40000 ALTER TABLE `ps_globals` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `ps_registrations`
--

DROP TABLE IF EXISTS `ps_registrations`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `ps_registrations` (
  `id` varchar(40) NOT NULL,
  `auth_rejection_permanent` enum('yes','no') DEFAULT NULL,
  `client_uri` varchar(255) DEFAULT NULL,
  `contact_user` varchar(40) DEFAULT NULL,
  `expiration` int(11) DEFAULT NULL,
  `max_retries` int(11) DEFAULT NULL,
  `outbound_auth` varchar(40) DEFAULT NULL,
  `outbound_proxy` varchar(40) DEFAULT NULL,
  `retry_interval` int(11) DEFAULT NULL,
  `forbidden_retry_interval` int(11) DEFAULT NULL,
  `server_uri` varchar(255) DEFAULT NULL,
  `transport` varchar(40) DEFAULT NULL,
  `support_path` enum('yes','no') DEFAULT NULL,
  `fatal_retry_interval` int(11) DEFAULT NULL,
  `line` enum('yes','no') DEFAULT NULL,
  `endpoint` varchar(40) DEFAULT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `id` (`id`),
  KEY `ps_registrations_id` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `ps_registrations`
--

LOCK TABLES `ps_registrations` WRITE;
/*!40000 ALTER TABLE `ps_registrations` DISABLE KEYS */;
/*!40000 ALTER TABLE `ps_registrations` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `ps_subscription_persistence`
--

DROP TABLE IF EXISTS `ps_subscription_persistence`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `ps_subscription_persistence` (
  `id` varchar(40) NOT NULL,
  `packet` varchar(2048) DEFAULT NULL,
  `src_name` varchar(128) DEFAULT NULL,
  `src_port` int(11) DEFAULT NULL,
  `transport_key` varchar(64) DEFAULT NULL,
  `local_name` varchar(128) DEFAULT NULL,
  `local_port` int(11) DEFAULT NULL,
  `cseq` int(11) DEFAULT NULL,
  `tag` varchar(128) DEFAULT NULL,
  `endpoint` varchar(40) DEFAULT NULL,
  `expires` int(11) DEFAULT NULL,
  UNIQUE KEY `id` (`id`),
  KEY `ps_subscription_persistence_id` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `ps_subscription_persistence`
--

LOCK TABLES `ps_subscription_persistence` WRITE;
/*!40000 ALTER TABLE `ps_subscription_persistence` DISABLE KEYS */;
/*!40000 ALTER TABLE `ps_subscription_persistence` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `ps_systems`
--

DROP TABLE IF EXISTS `ps_systems`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `ps_systems` (
  `id` varchar(40) NOT NULL,
  `timer_t1` int(11) DEFAULT NULL,
  `timer_b` int(11) DEFAULT NULL,
  `compact_headers` enum('yes','no') DEFAULT NULL,
  `threadpool_initial_size` int(11) DEFAULT NULL,
  `threadpool_auto_increment` int(11) DEFAULT NULL,
  `threadpool_idle_timeout` int(11) DEFAULT NULL,
  `threadpool_max_size` int(11) DEFAULT NULL,
  `disable_tcp_switch` enum('yes','no') DEFAULT NULL,
  UNIQUE KEY `id` (`id`),
  KEY `ps_systems_id` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `ps_systems`
--

LOCK TABLES `ps_systems` WRITE;
/*!40000 ALTER TABLE `ps_systems` DISABLE KEYS */;
/*!40000 ALTER TABLE `ps_systems` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `ps_transports`
--

DROP TABLE IF EXISTS `ps_transports`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `ps_transports` (
  `id` varchar(40) NOT NULL,
  `async_operations` int(11) DEFAULT NULL,
  `bind` varchar(40) DEFAULT NULL,
  `ca_list_file` varchar(200) DEFAULT NULL,
  `cert_file` varchar(200) DEFAULT NULL,
  `cipher` varchar(200) DEFAULT NULL,
  `domain` varchar(40) DEFAULT NULL,
  `external_media_address` varchar(40) DEFAULT NULL,
  `external_signaling_address` varchar(40) DEFAULT NULL,
  `external_signaling_port` int(11) DEFAULT NULL,
  `method` enum('default','unspecified','tlsv1','sslv2','sslv3','sslv23') DEFAULT NULL,
  `local_net` varchar(40) DEFAULT NULL,
  `password` varchar(40) DEFAULT NULL,
  `priv_key_file` varchar(200) DEFAULT NULL,
  `protocol` enum('udp','tcp','tls','ws','wss') DEFAULT NULL,
  `require_client_cert` enum('yes','no') DEFAULT NULL,
  `verify_client` enum('yes','no') DEFAULT NULL,
  `verify_server` enum('yes','no') DEFAULT NULL,
  `tos` varchar(10) DEFAULT NULL,
  `cos` int(11) DEFAULT NULL,
  `allow_reload` enum('yes','no') DEFAULT NULL,
  UNIQUE KEY `id` (`id`),
  KEY `ps_transports_id` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `ps_transports`
--

LOCK TABLES `ps_transports` WRITE;
/*!40000 ALTER TABLE `ps_transports` DISABLE KEYS */;
/*!40000 ALTER TABLE `ps_transports` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `queue_members`
--

DROP TABLE IF EXISTS `queue_members`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `queue_members` (
  `queue_name` varchar(80) NOT NULL,
  `interface` varchar(80) NOT NULL,
  `membername` varchar(80) DEFAULT NULL,
  `state_interface` varchar(80) DEFAULT NULL,
  `penalty` int(11) DEFAULT NULL,
  `paused` int(11) DEFAULT NULL,
  `uniqueid` int(11) NOT NULL AUTO_INCREMENT,
  `user_id` int(11) DEFAULT NULL,
  PRIMARY KEY (`queue_name`,`interface`),
  UNIQUE KEY `uniqueid` (`uniqueid`),
  KEY `FK_queue_members_users` (`user_id`),
  KEY `queue_name` (`queue_name`),
  CONSTRAINT `FK_queue_members_users` FOREIGN KEY (`user_id`) REFERENCES `users` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `queue_members`
--

LOCK TABLES `queue_members` WRITE;
/*!40000 ALTER TABLE `queue_members` DISABLE KEYS */;
/*!40000 ALTER TABLE `queue_members` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `queue_rules`
--

DROP TABLE IF EXISTS `queue_rules`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `queue_rules` (
  `rule_name` varchar(80) NOT NULL,
  `time` varchar(32) NOT NULL,
  `min_penalty` varchar(32) NOT NULL,
  `max_penalty` varchar(32) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `queue_rules`
--

LOCK TABLES `queue_rules` WRITE;
/*!40000 ALTER TABLE `queue_rules` DISABLE KEYS */;
/*!40000 ALTER TABLE `queue_rules` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `queues`
--

DROP TABLE IF EXISTS `queues`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `queues` (
  `name` varchar(128) NOT NULL,
  `musiconhold` varchar(128) DEFAULT NULL,
  `announce` varchar(128) DEFAULT NULL,
  `context` varchar(128) DEFAULT NULL,
  `timeout` int(11) DEFAULT NULL,
  `ringinuse` enum('yes','no') DEFAULT NULL,
  `setinterfacevar` enum('yes','no') DEFAULT NULL,
  `setqueuevar` enum('yes','no') DEFAULT NULL,
  `setqueueentryvar` enum('yes','no') DEFAULT NULL,
  `monitor_format` varchar(8) DEFAULT NULL,
  `membermacro` varchar(512) DEFAULT NULL,
  `membergosub` varchar(512) DEFAULT NULL,
  `queue_youarenext` varchar(128) DEFAULT NULL,
  `queue_thereare` varchar(128) DEFAULT NULL,
  `queue_callswaiting` varchar(128) DEFAULT NULL,
  `queue_quantity1` varchar(128) DEFAULT NULL,
  `queue_quantity2` varchar(128) DEFAULT NULL,
  `queue_holdtime` varchar(128) DEFAULT NULL,
  `queue_minutes` varchar(128) DEFAULT NULL,
  `queue_minute` varchar(128) DEFAULT NULL,
  `queue_seconds` varchar(128) DEFAULT NULL,
  `queue_thankyou` varchar(128) DEFAULT NULL,
  `queue_callerannounce` varchar(128) DEFAULT NULL,
  `queue_reporthold` varchar(128) DEFAULT NULL,
  `announce_frequency` int(11) DEFAULT NULL,
  `announce_to_first_user` enum('yes','no') DEFAULT NULL,
  `min_announce_frequency` int(11) DEFAULT NULL,
  `announce_round_seconds` int(11) DEFAULT NULL,
  `announce_holdtime` varchar(128) DEFAULT NULL,
  `announce_position` varchar(128) DEFAULT NULL,
  `announce_position_limit` int(11) DEFAULT NULL,
  `periodic_announce` varchar(50) DEFAULT NULL,
  `periodic_announce_frequency` int(11) DEFAULT NULL,
  `relative_periodic_announce` enum('yes','no') DEFAULT NULL,
  `random_periodic_announce` enum('yes','no') DEFAULT NULL,
  `retry` int(11) DEFAULT NULL,
  `wrapuptime` int(11) DEFAULT NULL,
  `penaltymemberslimit` int(11) DEFAULT NULL,
  `autofill` enum('yes','no') DEFAULT NULL,
  `monitor_type` varchar(128) DEFAULT NULL,
  `autopause` enum('yes','no','all') DEFAULT NULL,
  `autopausedelay` int(11) DEFAULT NULL,
  `autopausebusy` enum('yes','no') DEFAULT NULL,
  `autopauseunavail` enum('yes','no') DEFAULT NULL,
  `maxlen` int(11) DEFAULT NULL,
  `servicelevel` int(11) DEFAULT NULL,
  `strategy` enum('ringall','leastrecent','fewestcalls','random','rrmemory','linear','wrandom','rrordered') DEFAULT NULL,
  `joinempty` varchar(128) DEFAULT NULL,
  `leavewhenempty` varchar(128) DEFAULT NULL,
  `reportholdtime` enum('yes','no') DEFAULT NULL,
  `memberdelay` int(11) DEFAULT NULL,
  `weight` int(11) DEFAULT NULL,
  `timeoutrestart` enum('yes','no') DEFAULT NULL,
  `defaultrule` varchar(128) DEFAULT NULL,
  `timeoutpriority` varchar(128) DEFAULT NULL,
  PRIMARY KEY (`name`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `queues`
--

LOCK TABLES `queues` WRITE;
/*!40000 ALTER TABLE `queues` DISABLE KEYS */;
/*!40000 ALTER TABLE `queues` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `repair_status_logs`
--

DROP TABLE IF EXISTS `repair_status_logs`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `repair_status_logs` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `created_at` datetime NOT NULL,
  `repair_id` int(11) NOT NULL,
  `status_id` int(11) NOT NULL,
  `user_id` int(11) NOT NULL,
  `manager_id` int(11) DEFAULT NULL,
  `master_id` int(11) DEFAULT NULL,
  PRIMARY KEY (`id`) USING BTREE,
  KEY `FK__workshop` (`repair_id`) USING BTREE,
  KEY `FK__users` (`user_id`) USING BTREE,
  KEY `FK__users_2` (`manager_id`) USING BTREE,
  KEY `FK__users_3` (`master_id`) USING BTREE,
  KEY `created_at` (`created_at`) USING BTREE,
  CONSTRAINT `FK__users` FOREIGN KEY (`user_id`) REFERENCES `users` (`id`),
  CONSTRAINT `FK__users_2` FOREIGN KEY (`manager_id`) REFERENCES `users` (`id`),
  CONSTRAINT `FK__users_3` FOREIGN KEY (`master_id`) REFERENCES `users` (`id`),
  CONSTRAINT `FK__workshop` FOREIGN KEY (`repair_id`) REFERENCES `workshop` (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=3 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `repair_status_logs`
--

LOCK TABLES `repair_status_logs` WRITE;
/*!40000 ALTER TABLE `repair_status_logs` DISABLE KEYS */;
/*!40000 ALTER TABLE `repair_status_logs` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `roles`
--

DROP TABLE IF EXISTS `roles`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `roles` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(45) NOT NULL,
  `state` varchar(2) DEFAULT NULL COMMENT 'Состояние вкл/выкл/etc.',
  `description` varchar(254) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=7 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `roles`
--

LOCK TABLES `roles` WRITE;
/*!40000 ALTER TABLE `roles` DISABLE KEYS */;
INSERT INTO `roles` VALUES (1,'Администратор','1','Встроенная роль администратора'),(2,'Инженер','1','Встроенная роль мастера'),(3,'Менеджер','1','Встроенная роль продавца'),(4,'Директор','1','Роль администратора с минимальными ограничениями'),(5,'Старший инженер','1','Встроенная роль мастера с расширенными полномочиями'),(6,'старший менеджер ','1','менеджер расширенные права');
/*!40000 ALTER TABLE `roles` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `roles_users`
--

DROP TABLE IF EXISTS `roles_users`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `roles_users` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `user_id` int(11) NOT NULL,
  `role_id` int(11) NOT NULL,
  PRIMARY KEY (`id`),
  KEY `fk_roles_users_users1_idx` (`user_id`),
  KEY `fk_roles_users_roles1_idx` (`role_id`),
  CONSTRAINT `fk_roles_users_roles1` FOREIGN KEY (`role_id`) REFERENCES `roles` (`id`) ON DELETE CASCADE,
  CONSTRAINT `fk_roles_users_users1` FOREIGN KEY (`user_id`) REFERENCES `users` (`id`) ON DELETE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=7 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `roles_users`
--

LOCK TABLES `roles_users` WRITE;
/*!40000 ALTER TABLE `roles_users` DISABLE KEYS */;
INSERT INTO `roles_users` VALUES (1,1,1),(2,2,2),(3,3,3),(4,4,4),(5,5,5),(6,6,6);
/*!40000 ALTER TABLE `roles_users` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `salary`
--

DROP TABLE IF EXISTS `salary`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `salary` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `user_id` int(11) NOT NULL,
  `summ` decimal(19,4) NOT NULL,
  `balance` decimal(19,4) NOT NULL,
  `balance_record` int(11) DEFAULT NULL COMMENT 'разница между заработанными и выплаченными деньгами, записанная на баланс сотрдника',
  `payment_date` datetime NOT NULL,
  `notes` varchar(254) NOT NULL,
  `period_from` datetime NOT NULL,
  `period_to` datetime NOT NULL,
  `from_user` int(11) NOT NULL COMMENT 'Инициатор платежа',
  `type` tinyint(4) NOT NULL COMMENT '0 зп\n1 аванс',
  PRIMARY KEY (`id`),
  KEY `fk_salary_users1_idx` (`user_id`),
  KEY `fk_salary_users2_idx` (`from_user`),
  KEY `fk_salary_balance` (`balance_record`),
  CONSTRAINT `fk_salary_balance` FOREIGN KEY (`balance_record`) REFERENCES `balance` (`id`) ON UPDATE NO ACTION,
  CONSTRAINT `fk_salary_users1` FOREIGN KEY (`user_id`) REFERENCES `users` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT `fk_salary_users2` FOREIGN KEY (`from_user`) REFERENCES `users` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `salary`
--

LOCK TABLES `salary` WRITE;
/*!40000 ALTER TABLE `salary` DISABLE KEYS */;
/*!40000 ALTER TABLE `salary` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `salary_rates`
--

DROP TABLE IF EXISTS `salary_rates`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `salary_rates` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `created_at` datetime NOT NULL,
  `start_from` datetime NOT NULL,
  `user_id` int(11) NOT NULL,
  `value` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`) USING BTREE,
  KEY `FK_salary_rates_users` (`user_id`) USING BTREE,
  KEY `start_at` (`start_from`) USING BTREE,
  CONSTRAINT `FK_salary_rates_users` FOREIGN KEY (`user_id`) REFERENCES `users` (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `salary_rates`
--

LOCK TABLES `salary_rates` WRITE;
/*!40000 ALTER TABLE `salary_rates` DISABLE KEYS */;
/*!40000 ALTER TABLE `salary_rates` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `salary_repairs`
--

DROP TABLE IF EXISTS `salary_repairs`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `salary_repairs` (
  `id` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `accounting_date` datetime NOT NULL COMMENT 'дата учета (соответствует одному из дней в выбранном периоде, например, первому)',
  `repair` int(11) DEFAULT NULL COMMENT 'выплата за ремонт',
  `cartridge` int(11) DEFAULT NULL COMMENT 'выплата за картридж',
  `user` int(11) NOT NULL COMMENT 'id сотрудника, которому была сделана эта выплата',
  `summ` decimal(11,5) DEFAULT NULL COMMENT 'сумма, выплаченная сотруднику за работы и детали',
  PRIMARY KEY (`id`),
  KEY `FK_salary_repairs_c_workshop` (`cartridge`),
  KEY `FK_salary_repairs_users` (`user`),
  KEY `FK_salary_repairs_workshop` (`repair`),
  CONSTRAINT `FK_salary_repairs_c_workshop` FOREIGN KEY (`cartridge`) REFERENCES `c_workshop` (`id`),
  CONSTRAINT `FK_salary_repairs_users` FOREIGN KEY (`user`) REFERENCES `users` (`id`),
  CONSTRAINT `FK_salary_repairs_workshop` FOREIGN KEY (`repair`) REFERENCES `workshop` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COMMENT='Список ремонтов, за которые была выплачена заработная плата';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `salary_repairs`
--

LOCK TABLES `salary_repairs` WRITE;
/*!40000 ALTER TABLE `salary_repairs` DISABLE KEYS */;
/*!40000 ALTER TABLE `salary_repairs` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `schemaversions`
--

DROP TABLE IF EXISTS `schemaversions`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `schemaversions` (
  `schemaversionid` int(11) NOT NULL AUTO_INCREMENT,
  `scriptname` varchar(255) NOT NULL,
  `applied` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`schemaversionid`)
) ENGINE=InnoDB AUTO_INCREMENT=355 DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `schemaversions`
--

LOCK TABLES `schemaversions` WRITE;
/*!40000 ALTER TABLE `schemaversions` DISABLE KEYS */;
INSERT INTO `schemaversions` VALUES (1,'ASC.Scripts.Script00001.sql','2017-07-13 00:26:09'),(2,'ASC.Scripts.Script00002.sql','2017-07-13 00:33:56'),(3,'ASC.Scripts.Script00003.sql','2017-07-13 00:36:56'),(4,'ASC.Scripts.Script00004.sql','2017-09-25 15:37:15'),(5,'ASC.Scripts.Script00005.sql','2017-09-25 15:37:15'),(6,'ASC.Scripts.Script00006.sql','2017-09-25 15:37:15'),(7,'ASC.Scripts.Script00007.sql','2017-09-25 15:37:15'),(8,'ASC.Scripts.Script00008.sql','2017-09-25 15:37:15'),(9,'ASC.Scripts.Script00009.sql','2017-09-25 15:37:15'),(10,'ASC.Scripts.Script00010.sql','2017-09-25 15:37:15'),(11,'ASC.Scripts.Script00011.sql','2017-09-25 15:37:15'),(12,'ASC.Scripts.Script00012.sql','2017-10-28 14:50:40'),(13,'ASC.Scripts.Script00014.sql','2017-10-28 14:50:40'),(14,'ASC.Scripts.Script00015.sql','2017-10-28 14:50:40'),(15,'ASC.Scripts.Script00016.sql','2017-10-28 17:22:40'),(16,'ASC.Scripts.Script00017.sql','2017-10-28 17:22:40'),(17,'ASC.Scripts.Script00018.sql','2017-10-28 17:22:40'),(18,'ASC.Scripts.Script00019.sql','2017-10-28 17:22:40'),(19,'ASC.Scripts.Script00020.sql','2017-10-28 17:44:29'),(20,'ASC.Scripts.Script00021.sql','2017-11-21 23:59:31'),(21,'ASC.Scripts.Script00022.sql','2017-11-21 23:59:31'),(22,'ASC.Scripts.Script00023.sql','2017-11-21 23:59:31'),(23,'ASC.Scripts.Script00024.sql','2017-11-21 23:59:31'),(24,'ASC.Scripts.Script00025.sql','2017-11-21 23:59:31'),(25,'ASC.Scripts.Script00026.sql','2017-11-21 23:59:31'),(26,'ASC.Scripts.Script00027.sql','2017-11-29 17:43:33'),(27,'ASC.Scripts.Script000028.sql','2017-12-03 17:29:58'),(28,'ASC.Scripts.Script000029.sql','2017-12-03 17:32:08'),(29,'ASC.Scripts.Script000030.sql','2017-12-09 18:46:03'),(30,'ASC.Scripts.Script000031.sql','2017-12-09 18:46:03'),(31,'ASC.Scripts.Script000032.sql','2017-12-09 18:46:03'),(32,'ASC.Scripts.Script000033.sql','2017-12-11 15:36:18'),(33,'ASC.Scripts.Script000034.sql','2017-12-20 01:41:32'),(34,'ASC.Scripts.Script000035.sql','2017-12-21 19:04:09'),(35,'ASC.Scripts.Script000036.sql','2017-12-21 19:04:09'),(36,'ASC.Scripts.Script000037.sql','2018-01-02 15:15:05'),(37,'ASC.Scripts.Script000038.sql','2018-01-02 15:15:08'),(38,'ASC.Scripts.Script000039.sql','2018-01-09 01:47:40'),(39,'ASC.Scripts.Script000040.sql','2018-03-08 23:28:39'),(40,'ASC.Scripts.Script000041.sql','2018-03-08 23:31:20'),(41,'ASC.Scripts.Script000042.sql','2018-03-08 23:33:32'),(42,'ASC.Scripts.Script000043.sql','2018-03-08 23:35:25'),(43,'ASC.Scripts.Script000044.sql','2018-03-08 23:37:19'),(44,'ASC.Scripts.Script000045.sql','2018-03-08 23:39:07'),(45,'ASC.Scripts.Script000046.sql','2018-03-08 21:42:37'),(46,'ASC.Scripts.Script000047.sql','2018-03-08 23:44:47'),(47,'ASC.Scripts.Script000048.sql','2018-03-08 21:46:55'),(48,'ASC.Scripts.Script000049.sql','2018-03-08 23:34:54'),(49,'ASC.Scripts.Script000049.sql','2018-03-11 17:56:57'),(50,'ASC.Scripts.Script000049.sql','2018-03-11 18:37:52'),(51,'ASC.Scripts.Script000050.sql','2018-03-12 01:08:44'),(52,'ASC.Scripts.Script000051.sql','2018-06-05 22:34:57'),(53,'ASC.Scripts.Script000052.sql','2018-06-06 01:44:10'),(54,'ASC.Scripts.Script000053.sql','2018-06-06 01:44:11'),(55,'ASC.Scripts.Script000054.sql','2018-06-06 01:44:12'),(56,'ASC.Scripts.Script000055.sql','2018-08-05 03:56:17'),(57,'ASC.Scripts.Script000056.sql','2018-08-05 03:56:17'),(58,'ASC.Scripts.Script000057.sql','2018-08-05 03:56:17'),(59,'ASC.Scripts.Script000058.sql','2018-08-05 03:56:17'),(60,'ASC.Scripts.Script000059.sql','2018-08-05 03:56:17'),(61,'ASC.Scripts.Script000060.sql','2018-08-05 03:56:17'),(62,'ASC.Scripts.Script000061.sql','2018-08-05 03:56:18'),(63,'ASC.Scripts.Script000062.sql','2018-08-05 03:56:18'),(64,'ASC.Scripts.Script000063.sql','2018-09-27 01:48:10'),(65,'ASC.Scripts.Script000064.sql','2018-09-27 01:48:11'),(66,'ASC.Scripts.Script000065.sql','2018-09-27 01:48:11'),(67,'ASC.Scripts.Script000066.sql','2018-09-27 01:48:11'),(68,'ASC.Scripts.Script000067.sql','2018-09-27 01:48:11'),(69,'ASC.Scripts.Script000068.sql','2018-09-27 01:48:11'),(70,'ASC.Scripts.Script000069.sql','2018-09-27 01:48:11'),(71,'ASC.Scripts.Script000070.sql','2018-09-27 01:48:11'),(72,'ASC.Scripts.Script000071.sql','2018-09-27 01:48:11'),(73,'ASC.Scripts.Script000072.sql','2018-09-27 01:48:11'),(74,'ASC.Scripts.Script000073.sql','2018-09-27 01:48:12'),(75,'ASC.Scripts.Script000074.sql','2018-10-04 02:04:04'),(76,'ASC.Scripts.Script000075.sql','2018-10-04 02:04:04'),(77,'ASC.Scripts.Script000076.sql','2018-10-04 02:04:04'),(78,'ASC.Scripts.Script000077.sql','2018-10-04 02:04:04'),(79,'ASC.Scripts.Script000078.sql','2018-10-09 09:38:32'),(80,'ASC.Scripts.Script000079.sql','2018-10-09 09:38:32'),(81,'ASC.Scripts.Script000080.sql','2018-10-17 01:15:28'),(82,'ASC.Scripts.Script000081.sql','2018-10-17 01:15:28'),(83,'ASC.Scripts.Script000082.sql','2018-10-24 22:31:42'),(84,'ASC.Scripts.Script000083.sql','2018-10-27 10:51:52'),(85,'ASC.Scripts.Script000084.sql','2018-12-02 16:44:48'),(86,'ASC.Scripts.Script000085.sql','2018-12-02 16:44:48'),(87,'ASC.Scripts.Script000086.sql','2018-12-02 16:44:48'),(88,'ASC.Scripts.Script000087.sql','2018-12-02 16:44:48'),(89,'ASC.Scripts.Script000088.sql','2018-12-02 16:44:48'),(90,'ASC.Scripts.Script000089.sql','2018-12-02 16:44:48'),(91,'ASC.Scripts.Script000090.sql','2018-12-02 16:44:48'),(92,'ASC.Scripts.Script000091.sql','2018-12-21 10:37:27'),(93,'ASC.Scripts.Script000092.sql','2018-12-21 10:37:27'),(94,'ASC.Scripts.Script000093.sql','2018-12-21 10:37:27'),(95,'ASC.Scripts.Script000094.sql','2018-12-21 10:37:27'),(96,'ASC.Scripts.Script000095.sql','2018-12-21 10:37:28'),(97,'ASC.Scripts.Script000096.sql','2018-12-21 10:37:28'),(98,'ASC.Scripts.Script000097.sql','2018-12-21 10:37:28'),(99,'ASC.Scripts.Script000098.sql','2019-01-20 14:13:23'),(100,'ASC.Scripts.Script000099.sql','2019-01-20 14:13:24'),(101,'ASC.Scripts.Script000100.sql','2019-01-20 14:13:24'),(102,'ASC.Scripts.Script000101.sql','2019-01-20 14:13:24'),(103,'ASC.Scripts.Script000102.sql','2019-01-20 14:13:24'),(104,'ASC.Scripts.Script000103.sql','2019-01-20 14:13:24'),(105,'ASC.Scripts.Script000104.sql','2019-01-20 14:13:24'),(106,'ASC.Scripts.Script000105.sql','2019-01-20 14:13:24'),(107,'ASC.Scripts.Script000106.sql','2019-01-20 14:13:24'),(108,'ASC.Scripts.Script000107.sql','2019-01-20 14:13:24'),(109,'ASC.Scripts.Script000108.sql','2019-01-28 09:55:21'),(110,'ASC.Scripts.Script000109.sql','2019-01-28 09:55:21'),(111,'ASC.Scripts.Script000110.sql','2019-01-28 09:55:21'),(112,'ASC.Scripts.Script000111.sql','2019-01-28 09:55:21'),(113,'ASC.Scripts.Script000112.sql','2019-01-28 09:55:21'),(114,'ASC.Scripts.Script000113.sql','2019-01-28 09:55:21'),(115,'ASC.Scripts.Script000114.sql','2019-01-28 09:55:21'),(116,'ASC.Scripts.Script000115.sql','2019-02-11 21:53:18'),(117,'ASC.Scripts.Script000116.sql','2019-02-11 21:53:18'),(118,'ASC.Scripts.Script000117.sql','2019-02-11 21:53:18'),(119,'ASC.Scripts.Script000118.sql','2019-02-11 21:53:18'),(120,'ASC.Scripts.Script000119.sql','2019-02-11 21:53:18'),(121,'ASC.Scripts.Script000120.sql','2019-02-11 21:53:18'),(122,'ASC.Scripts.Script000121.sql','2019-02-17 16:48:25'),(123,'ASC.Scripts.Script000122.sql','2019-02-17 16:48:25'),(124,'ASC.Scripts.Script000123.sql','2019-02-17 16:48:25'),(125,'ASC.Scripts.Script000124.sql','2019-02-17 16:48:25'),(126,'ASC.Scripts.Script000125-0.sql','2019-03-20 17:45:47'),(127,'ASC.Scripts.Script000125.sql','2019-03-20 17:45:48'),(128,'ASC.Scripts.Script000126.sql','2019-03-20 17:45:48'),(129,'ASC.Scripts.Script000127.sql','2019-03-20 17:45:48'),(130,'ASC.Scripts.Script000128.sql','2019-03-20 17:45:48'),(131,'ASC.Scripts.Script000129.sql','2019-03-20 17:45:48'),(132,'ASC.Scripts.Script000130.sql','2019-03-20 17:45:48'),(133,'ASC.Scripts.Script000131.sql','2019-03-20 17:45:48'),(134,'ASC.Scripts.Script000132.sql','2019-03-20 17:45:48'),(135,'ASC.Scripts.Script000133.sql','2019-03-20 17:45:48'),(136,'ASC.Scripts.Script000134.sql','2019-03-23 00:10:07'),(137,'ASC.Scripts.Script000135.sql','2019-03-23 00:10:07'),(138,'ASC.Scripts.Script000136.sql','2019-03-23 00:10:07'),(139,'ASC.Scripts.Script000137.sql','2019-03-23 00:10:07'),(140,'ASC.Scripts.Script000138.sql','2019-03-23 00:10:07'),(141,'ASC.Scripts.Script000139.sql','2019-03-23 00:10:07'),(142,'ASC.Scripts.Script000140.sql','2019-03-23 00:10:07'),(143,'ASC.Scripts.Script000141.sql','2019-04-22 02:37:43'),(144,'ASC.Scripts.Script000142.sql','2019-04-22 02:37:43'),(145,'ASC.Scripts.Script000143.sql','2019-04-22 02:37:43'),(146,'ASC.Scripts.Script000144.sql','2019-04-22 02:37:43'),(147,'ASC.Scripts.Script000145.sql','2019-04-22 02:37:45'),(148,'ASC.Scripts.Script000146.sql','2019-04-22 02:37:45'),(149,'ASC.Scripts.Script000147.sql','2019-06-08 12:21:08'),(150,'ASC.Scripts.Script000148.sql','2019-06-08 12:21:09'),(151,'ASC.Scripts.Script000149.sql','2019-06-08 12:21:09'),(152,'ASC.Scripts.Script000150.sql','2019-06-08 12:21:09'),(153,'ASC.Scripts.Script000151.sql','2019-06-08 12:21:09'),(154,'ASC.Scripts.Script000152.sql','2019-06-08 12:21:09'),(155,'ASC.Scripts.Script000153.sql','2019-06-08 12:21:09'),(156,'ASC.Scripts.Script000154.sql','2019-06-08 12:21:09'),(157,'ASC.Scripts.Script000155.sql','2019-06-08 12:21:09'),(158,'ASC.Scripts.Script000156.sql','2019-06-08 12:21:09'),(159,'ASC.Scripts.Script000157.sql','2019-06-08 12:21:10'),(160,'ASC.Scripts.Script000158.sql','2019-06-08 12:21:11'),(161,'ASC.Scripts.Script000159.sql','2019-06-08 12:21:11'),(162,'ASC.Scripts.Script000160.sql','2019-06-08 12:21:11'),(163,'ASC.Scripts.Script000161.sql','2019-06-08 12:21:11'),(164,'ASC.Scripts.Script000162.sql','2019-06-08 12:21:11'),(165,'ASC.Scripts.Script000163.sql','2019-06-12 14:13:19'),(166,'ASC.Scripts.Script000164.sql','2019-08-07 10:20:27'),(167,'ASC.Scripts.Script000165.sql','2019-08-07 10:20:27'),(168,'ASC.Scripts.Script000166.sql','2019-08-07 10:20:27'),(169,'ASC.Scripts.Script000167.sql','2019-08-07 10:20:27'),(170,'ASC.Scripts.Script000168.sql','2019-08-07 10:20:27'),(171,'ASC.Scripts.Script000169.sql','2019-08-07 10:20:27'),(172,'ASC.Scripts.Script000170.sql','2019-08-07 10:20:27'),(173,'ASC.Scripts.Script000171.sql','2019-10-27 14:24:58'),(174,'ASC.Scripts.Script000172.sql','2019-10-27 14:24:58'),(175,'ASC.Scripts.Script000173.sql','2019-10-27 14:24:58'),(176,'ASC.Scripts.Script000174.sql','2019-10-27 14:24:58'),(177,'ASC.Scripts.Script000175.sql','2019-10-27 14:24:58'),(178,'ASC.Scripts.Script000176.sql','2019-10-27 14:24:58'),(179,'ASC.Scripts.Script000177.sql','2019-10-27 14:24:58'),(180,'ASC.Scripts.Script000178.sql','2019-10-27 14:24:58'),(181,'ASC.Scripts.Script000179.sql','2019-10-27 14:24:58'),(182,'ASC.Scripts.Script000180.sql','2019-10-27 14:24:58'),(183,'ASC.Scripts.Script000181.sql','2019-10-27 14:24:58'),(184,'ASC.Scripts.Script000182.sql','2019-10-27 14:24:58'),(185,'ASC.Scripts.Script000183.sql','2019-10-27 14:24:58'),(186,'ASC.Scripts.Script000184.sql','2019-10-27 14:24:58'),(187,'ASC.Scripts.Script000185.sql','2019-10-27 14:24:58'),(188,'ASC.Scripts.Script000186.sql','2019-10-27 14:24:58'),(189,'ASC.Scripts.Script000187.sql','2019-10-27 14:24:58'),(190,'ASC.Scripts.Script000188.sql','2019-10-27 14:24:58'),(191,'ASC.Scripts.Script000189.sql','2019-10-27 14:24:58'),(192,'ASC.Scripts.Script000190.sql','2019-10-27 14:24:58'),(193,'ASC.Scripts.Script000191.sql','2019-10-27 14:24:58'),(194,'ASC.Scripts.Script000192.sql','2019-10-27 14:24:58'),(195,'ASC.Scripts.Script000193.sql','2019-10-27 14:24:58'),(196,'ASC.Scripts.Script000194.sql','2019-10-27 14:24:58'),(197,'ASC.Scripts.Script000195.sql','2019-10-27 14:24:58'),(198,'ASC.Scripts.Script000196.sql','2019-10-27 14:24:58'),(199,'ASC.Scripts.Script000197.sql','2019-10-27 14:24:58'),(200,'ASC.Scripts.Script000198.sql','2019-10-27 14:24:58'),(201,'ASC.Scripts.Script000199.sql','2019-12-08 18:05:11'),(202,'ASC.Scripts.Script000200.sql','2019-12-08 18:05:11'),(203,'ASC.Scripts.Script000201.sql','2019-12-08 18:05:11'),(204,'ASC.Scripts.Script000202.sql','2019-12-08 18:05:12'),(205,'ASC.Scripts.Script000203.sql','2019-12-08 18:05:12'),(206,'ASC.Scripts.Script000204.sql','2020-01-14 15:02:52'),(207,'ASC.Scripts.Script000205.sql','2020-01-14 15:02:52'),(208,'ASC.Scripts.Script000204.sql','2020-03-08 16:50:50'),(209,'ASC.Scripts.Script000205.sql','2020-03-08 16:50:50'),(210,'ASC.Scripts.Script000206.sql','2020-03-08 16:50:51'),(211,'ASC.Scripts.Script000207.sql','2020-03-08 16:50:52'),(212,'ASC.Scripts.Script000208.sql','2020-03-08 16:50:52'),(213,'ASC.Scripts.Script000209.sql','2020-03-08 16:50:52'),(214,'ASC.Scripts.Script000210.sql','2020-03-08 16:50:52'),(215,'ASC.Scripts.Script000211.sql','2020-03-08 16:50:52'),(216,'ASC.Scripts.Script000212.sql','2020-03-08 16:50:54'),(217,'ASC.Scripts.Script000213.sql','2020-03-08 16:50:54'),(218,'ASC.Scripts.Script000214.sql','2020-03-08 16:50:54'),(219,'ASC.Scripts.Script000215.sql','2020-03-08 16:50:54'),(220,'ASC.Scripts.Script000216.sql','2021-01-02 01:56:16'),(221,'ASC.Scripts.Script000217.sql','2021-01-02 01:56:16'),(222,'ASC.Scripts.Script000218.sql','2021-01-02 01:56:16'),(223,'ASC.Scripts.Script000219.sql','2021-01-02 01:56:16'),(224,'ASC.Scripts.Script000220.sql','2021-01-02 01:56:16'),(225,'ASC.Scripts.Script000221.sql','2021-01-02 01:56:16'),(226,'ASC.Scripts.Script000222.sql','2021-01-02 01:56:16'),(227,'ASC.Scripts.Script000223.sql','2021-01-02 01:56:17'),(228,'ASC.Scripts.Script000224.sql','2021-01-02 01:56:17'),(229,'ASC.Scripts.Script000225.sql','2021-01-02 01:56:17'),(230,'ASC.Scripts.Script000226.sql','2021-01-02 01:56:17'),(231,'ASC.Scripts.Script000227.sql','2021-01-02 01:56:17'),(232,'ASC.Scripts.Script000228.sql','2021-01-02 01:56:17'),(233,'ASC.Scripts.Script000229.sql','2021-01-02 01:56:17'),(234,'ASC.Scripts.Script000230.sql','2021-01-02 01:56:20'),(235,'ASC.Scripts.Script000231.sql','2021-01-02 01:56:20'),(236,'ASC.Scripts.Script000232.sql','2021-01-02 01:56:22'),(237,'ASC.Scripts.Script000233.sql','2021-01-02 01:56:22'),(238,'ASC.Scripts.Script000234.sql','2021-01-02 01:56:22'),(239,'ASC.Scripts.Script000235.sql','2021-01-02 01:56:22'),(240,'ASC.Scripts.Script000236.sql','2021-01-02 01:56:22'),(241,'ASC.Scripts.Script000237.sql','2021-01-02 01:56:22'),(242,'ASC.Scripts.Script000238.sql','2021-01-02 01:56:22'),(243,'ASC.Scripts.Script000239.sql','2021-01-02 01:56:22'),(244,'ASC.Scripts.Script000240.sql','2021-01-02 01:56:22'),(245,'ASC.Scripts.Script000241.sql','2021-01-02 01:56:22'),(246,'ASC.Scripts.Script000242.sql','2021-01-02 01:56:23'),(247,'ASC.Scripts.Script000243.sql','2021-01-02 01:56:24'),(248,'ASC.Scripts.Script000244.sql','2021-01-02 01:56:24'),(249,'ASC.Scripts.Script000245.sql','2021-01-02 01:56:24'),(250,'ASC.Scripts.Script000246.sql','2021-01-02 01:56:24'),(251,'ASC.Scripts.Script000247.sql','2021-01-02 01:56:24'),(252,'ASC.Scripts.Script000248.sql','2021-01-02 01:56:24'),(253,'ASC.Scripts.Script000249.sql','2021-01-02 01:56:24'),(254,'ASC.Scripts.Script000250.sql','2021-01-02 01:56:24'),(255,'ASC.Scripts.Script000251.sql','2021-01-02 01:56:24'),(256,'ASC.Scripts.Script000252.sql','2021-01-02 01:56:24'),(257,'ASC.Scripts.Script000253.sql','2021-01-02 01:56:24'),(258,'ASC.Scripts.Script000254.sql','2021-08-09 01:28:01'),(259,'ASC.Scripts.Script000255.sql','2021-08-09 01:28:01'),(260,'ASC.Scripts.Script000256.sql','2021-08-09 01:28:01'),(261,'ASC.Scripts.Script000257.sql','2021-08-09 01:28:01'),(262,'ASC.Scripts.Script000258.sql','2021-08-09 01:28:01'),(263,'ASC.Scripts.Script000259.sql','2021-08-09 01:28:01'),(264,'ASC.Scripts.Script000260.sql','2021-08-09 01:28:01'),(265,'ASC.Scripts.Script000261.sql','2021-08-09 01:28:01'),(266,'ASC.Scripts.Script000262.sql','2021-08-09 01:28:01'),(267,'ASC.Scripts.Script000263.sql','2021-08-09 01:28:01'),(268,'ASC.Scripts.Script000264.sql','2021-08-09 01:28:01'),(269,'ASC.Scripts.Script000265.sql','2021-08-09 01:28:02'),(270,'ASC.Scripts.Script000266.sql','2021-08-09 01:28:02'),(271,'ASC.Scripts.Script000267.sql','2021-08-09 01:28:02'),(272,'ASC.Scripts.Script000268.sql','2021-08-09 01:28:02'),(273,'ASC.Scripts.Script000269.sql','2021-08-09 01:28:02'),(274,'ASC.Scripts.Script000270.sql','2021-08-09 01:28:02'),(275,'ASC.Scripts.Script000271.sql','2021-08-09 01:28:02'),(276,'ASC.Scripts.Script000272.sql','2021-08-09 01:28:02'),(277,'ASC.Scripts.Script000273.sql','2021-08-09 01:28:02'),(278,'ASC.Scripts.Script000274.sql','2021-08-09 01:28:02'),(279,'ASC.Scripts.Script000275.sql','2021-08-09 01:28:02'),(280,'ASC.Scripts.Script000276.sql','2021-08-09 01:28:02'),(281,'ASC.Scripts.Script000277.sql','2021-08-09 01:28:02'),(282,'ASC.Scripts.Script000278.sql','2021-08-09 01:28:02'),(283,'ASC.Scripts.Script000279.sql','2021-08-09 01:28:02'),(284,'ASC.Scripts.Script000280.sql','2021-08-09 01:28:02'),(285,'ASC.Scripts.Script000281.sql','2021-08-09 01:28:02'),(286,'ASC.Scripts.Script000282.sql','2021-08-09 01:28:02'),(287,'ASC.Scripts.Script000283.sql','2021-08-09 01:28:02'),(288,'ASC.Scripts.Script000284.sql','2021-08-09 01:28:02'),(289,'ASC.Scripts.Script000285.sql','2021-08-09 01:28:02'),(290,'ASC.Scripts.Script000286.sql','2021-08-09 01:28:02'),(291,'ASC.Scripts.Script000287.sql','2021-08-09 01:28:02'),(292,'ASC.Scripts.Script000288.sql','2021-08-09 01:28:02'),(293,'ASC.Scripts.Script000289.sql','2021-08-09 01:28:02'),(294,'ASC.Scripts.Script000290.sql','2021-08-09 01:28:02'),(295,'ASC.Scripts.Script000291.sql','2021-08-09 01:28:02'),(296,'ASC.Scripts.Script000292.sql','2021-08-09 01:28:02'),(297,'ASC.Scripts.Script000293.sql','2021-08-09 01:28:02'),(298,'ASC.Scripts.Script000294.sql','2021-08-09 01:28:02'),(299,'ASC.Scripts.Script000295.sql','2021-08-09 01:28:02'),(300,'ASC.Scripts.Script000296.sql','2021-08-09 01:28:02'),(301,'ASC.Scripts.Script000297.sql','2021-08-09 01:28:02'),(302,'ASC.Scripts.Script000298.sql','2021-08-09 01:28:02'),(303,'ASC.Scripts.Script000299.sql','2021-08-09 01:28:02'),(304,'ASC.Scripts.Script000300.sql','2021-08-09 01:28:02'),(305,'ASC.Scripts.Script000301.sql','2021-08-09 01:28:02'),(306,'ASC.Scripts.Script000302.sql','2021-08-09 01:28:02'),(307,'ASC.Scripts.Script000303.sql','2021-08-09 01:28:02'),(308,'ASC.Scripts.Script000304.sql','2021-08-09 01:28:02'),(309,'ASC.Scripts.Script000305.sql','2021-08-09 01:28:02'),(310,'ASC.Scripts.Script000306.sql','2021-08-09 01:28:02'),(311,'ASC.Scripts.Script000307.sql','2021-08-09 01:28:02'),(312,'SNAP.schema-updates.script000001.sql','2022-10-26 20:11:08'),(313,'SNAP.schema-updates.script000002.sql','2022-10-26 20:11:08'),(314,'SNAP.schema-updates.script000003.sql','2022-10-26 20:11:09'),(315,'SNAP.schema-updates.script000004.sql','2022-10-26 20:11:09'),(316,'SNAP.schema-updates.script000005.sql','2023-01-11 22:50:01'),(317,'ASC.Scripts.Script000308.sql','2023-02-01 00:34:08'),(318,'ASC.Scripts.Script000309.sql','2023-02-01 00:34:08'),(319,'ASC.Scripts.Script000310.sql','2023-02-01 00:34:08'),(320,'ASC.Scripts.Script000311.sql','2023-02-01 00:34:08'),(321,'ASC.Scripts.Script000312.sql','2023-02-01 00:34:08'),(322,'ASC.Scripts.Script000313.sql','2023-02-01 00:34:08'),(323,'ASC.Scripts.Script000314.sql','2023-02-01 00:34:08'),(324,'ASC.Scripts.Script000315.sql','2023-02-01 00:34:08'),(325,'ASC.Scripts.Script000316.sql','2023-02-01 00:34:08'),(326,'ASC.Scripts.Script000317.sql','2023-02-01 00:34:08'),(327,'ASC.Scripts.Script000318.sql','2023-02-01 00:34:08'),(328,'ASC.Scripts.Script000319.sql','2023-02-01 00:34:08'),(329,'ASC.Scripts.Script000320.sql','2023-02-01 00:34:08'),(330,'ASC.Scripts.Script000321.sql','2023-02-01 00:34:08'),(331,'ASC.Scripts.Script000322.sql','2023-02-01 00:34:08'),(332,'ASC.Scripts.Script000323.sql','2023-02-01 00:34:08'),(333,'ASC.Scripts.Script000324.sql','2023-02-01 00:34:08'),(334,'ASC.Scripts.Script000325.sql','2023-02-01 00:34:08'),(335,'ASC.Scripts.Script000326.sql','2023-02-01 00:34:08'),(336,'SNAP.schema-updates.script000006.sql','2023-04-11 06:40:07'),(337,'SNAP.schema-updates.script000007.sql','2023-04-14 09:28:48'),(338,'SNAP.schema-updates.script000008.sql','2023-04-14 09:28:48'),(339,'SNAP.schema-updates.script000009.sql','2023-08-07 10:41:43'),(340,'SNAP.schema-updates.script000010.sql','2023-08-07 10:41:43'),(341,'SNAP.schema-updates.script000011.sql','2023-08-07 10:41:43'),(342,'SNAP.schema-updates.script000012.sql','2023-08-07 10:41:43'),(343,'SNAP.schema-updates.script000013.sql','2023-08-07 10:41:43'),(344,'SNAP.schema-updates.script000014.sql','2023-10-16 00:59:18'),(345,'SNAP.schema-updates.script000015.sql','2023-10-16 00:59:18'),(346,'SNAP.schema-updates.script000016.sql','2023-10-16 00:59:18'),(347,'SNAP.schema-updates.script000017.sql','2023-10-27 23:12:13'),(348,'SNAP.schema-updates.script000018.sql','2023-11-10 06:50:43'),(349,'SNAP.schema-updates.script000019.sql','2023-11-19 14:42:31'),(350,'SNAP.schema-updates.script000020.sql','2023-11-25 00:36:47'),(351,'SNAP.schema-updates.script000021.sql','2024-01-10 13:22:29'),(352,'SNAP.schema-updates.script000022.sql','2024-10-11 12:14:11'),(353,'SNAP.schema-updates.script000023.sql','2024-10-11 12:14:11'),(354,'SNAP.TEST','2025-11-17 23:53:46');
/*!40000 ALTER TABLE `schemaversions` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `settings`
--

DROP TABLE IF EXISTS `settings`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `settings` (
  `name` varchar(50) NOT NULL,
  `value` text NOT NULL,
  PRIMARY KEY (`name`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `settings`
--

LOCK TABLES `settings` WRITE;
/*!40000 ALTER TABLE `settings` DISABLE KEYS */;
INSERT INTO `settings` VALUES ('autosave_part_list','0'),('auto_assign_criteria','{\"StatusList\":[0],\"DayLimit\":4}'),('auto_assign_users','[2,5,7]'),('classic_salary','1'),('inform_call','1'),('inform_call_color','#FFCCCCCC'),('inform_comment','0'),('inform_comment_color','#FFCCCCCC'),('inform_int_request','1'),('inform_int_request_color','#FFCCCCCC'),('inform_part_request','1'),('inform_part_request_color','#FFCCCCCC'),('inform_sms','0'),('inform_sms_color','#FFCCCCCC'),('inform_status_color','#FFCCCCCC'),('inform_task_custom','1'),('inform_task_custom_color','#FFCCCCCC'),('inform_task_match','1'),('inform_task_match_color','#FFCCCCCC'),('inform_task_request','1'),('inform_task_request_color','#FFCCCCCC'),('inform_terms_color','#FFCCCCCC'),('new_client_sms_enabled','0'),('print_cartridge_stickers','0'),('print_new_cartridge_report','0'),('print_new_repair_report','0'),('print_rep_stickers','1'),('salary_include_not_issued_by_default','1'),('update_channel','https://github.com/solderercb/snap_crm/releases/latest/download'),('use_simplified_cartridge_repair','0'),('voip',''),('voip_endpoint',''),('voip_key',''),('voip_prefix','7'),('voip_secret','');
/*!40000 ALTER TABLE `settings` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `sippeers`
--

DROP TABLE IF EXISTS `sippeers`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `sippeers` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(40) NOT NULL,
  `ipaddr` varchar(45) DEFAULT NULL,
  `port` int(11) DEFAULT NULL,
  `regseconds` int(11) DEFAULT NULL,
  `defaultuser` varchar(40) DEFAULT NULL,
  `fullcontact` varchar(80) DEFAULT NULL,
  `regserver` varchar(20) DEFAULT NULL,
  `useragent` varchar(255) DEFAULT NULL,
  `lastms` int(11) DEFAULT NULL,
  `host` varchar(40) DEFAULT NULL,
  `type` enum('friend','user','peer') DEFAULT NULL,
  `context` varchar(40) DEFAULT NULL,
  `permit` varchar(95) DEFAULT NULL,
  `deny` varchar(95) DEFAULT NULL,
  `secret` varchar(40) DEFAULT NULL,
  `md5secret` varchar(40) DEFAULT NULL,
  `remotesecret` varchar(40) DEFAULT NULL,
  `transport` enum('udp','tcp','tls','ws','wss','udp,tcp','tcp,udp') DEFAULT NULL,
  `dtmfmode` enum('rfc2833','info','shortinfo','inband','auto') DEFAULT NULL,
  `directmedia` enum('yes','no','nonat','update','outgoing') DEFAULT NULL,
  `nat` varchar(29) DEFAULT NULL,
  `callgroup` varchar(40) DEFAULT NULL,
  `pickupgroup` varchar(40) DEFAULT NULL,
  `language` varchar(40) DEFAULT NULL,
  `disallow` varchar(200) DEFAULT NULL,
  `allow` varchar(200) DEFAULT NULL,
  `insecure` varchar(40) DEFAULT NULL,
  `trustrpid` enum('yes','no') DEFAULT NULL,
  `progressinband` enum('yes','no','never') DEFAULT NULL,
  `promiscredir` enum('yes','no') DEFAULT NULL,
  `useclientcode` enum('yes','no') DEFAULT NULL,
  `accountcode` varchar(80) DEFAULT NULL,
  `setvar` varchar(200) DEFAULT NULL,
  `callerid` varchar(40) DEFAULT NULL,
  `amaflags` varchar(40) DEFAULT NULL,
  `callcounter` enum('yes','no') DEFAULT NULL,
  `busylevel` int(11) DEFAULT NULL,
  `allowoverlap` enum('yes','no') DEFAULT NULL,
  `allowsubscribe` enum('yes','no') DEFAULT NULL,
  `videosupport` enum('yes','no') DEFAULT NULL,
  `maxcallbitrate` int(11) DEFAULT NULL,
  `rfc2833compensate` enum('yes','no') DEFAULT NULL,
  `mailbox` varchar(40) DEFAULT NULL,
  `session-timers` enum('accept','refuse','originate') DEFAULT NULL,
  `session-expires` int(11) DEFAULT NULL,
  `session-minse` int(11) DEFAULT NULL,
  `session-refresher` enum('uac','uas') DEFAULT NULL,
  `t38pt_usertpsource` varchar(40) DEFAULT NULL,
  `regexten` varchar(40) DEFAULT NULL,
  `fromdomain` varchar(40) DEFAULT NULL,
  `fromuser` varchar(40) DEFAULT NULL,
  `qualify` varchar(40) DEFAULT NULL,
  `defaultip` varchar(45) DEFAULT NULL,
  `rtptimeout` int(11) DEFAULT NULL,
  `rtpholdtimeout` int(11) DEFAULT NULL,
  `sendrpid` enum('yes','no') DEFAULT NULL,
  `outboundproxy` varchar(40) DEFAULT NULL,
  `callbackextension` varchar(40) DEFAULT NULL,
  `timert1` int(11) DEFAULT NULL,
  `timerb` int(11) DEFAULT NULL,
  `qualifyfreq` int(11) DEFAULT NULL,
  `constantssrc` enum('yes','no') DEFAULT NULL,
  `contactpermit` varchar(95) DEFAULT NULL,
  `contactdeny` varchar(95) DEFAULT NULL,
  `usereqphone` enum('yes','no') DEFAULT NULL,
  `textsupport` enum('yes','no') DEFAULT NULL,
  `faxdetect` enum('yes','no') DEFAULT NULL,
  `buggymwi` enum('yes','no') DEFAULT NULL,
  `auth` varchar(40) DEFAULT NULL,
  `fullname` varchar(40) DEFAULT NULL,
  `trunkname` varchar(40) DEFAULT NULL,
  `cid_number` varchar(40) DEFAULT NULL,
  `callingpres` enum('allowed_not_screened','allowed_passed_screen','allowed_failed_screen','allowed','prohib_not_screened','prohib_passed_screen','prohib_failed_screen','prohib') DEFAULT NULL,
  `mohinterpret` varchar(40) DEFAULT NULL,
  `mohsuggest` varchar(40) DEFAULT NULL,
  `parkinglot` varchar(40) DEFAULT NULL,
  `hasvoicemail` enum('yes','no') DEFAULT NULL,
  `subscribemwi` enum('yes','no') DEFAULT NULL,
  `vmexten` varchar(40) DEFAULT NULL,
  `autoframing` enum('yes','no') DEFAULT NULL,
  `rtpkeepalive` int(11) DEFAULT NULL,
  `call-limit` int(11) DEFAULT NULL,
  `g726nonstandard` enum('yes','no') DEFAULT NULL,
  `ignoresdpversion` enum('yes','no') DEFAULT NULL,
  `allowtransfer` enum('yes','no') DEFAULT NULL,
  `dynamic` enum('yes','no') DEFAULT NULL,
  `path` varchar(256) DEFAULT NULL,
  `supportpath` enum('yes','no') DEFAULT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `name` (`name`),
  KEY `sippeers_name` (`name`),
  KEY `sippeers_name_host` (`name`,`host`),
  KEY `sippeers_ipaddr_port` (`ipaddr`,`port`),
  KEY `sippeers_host_port` (`host`,`port`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `sippeers`
--

LOCK TABLES `sippeers` WRITE;
/*!40000 ALTER TABLE `sippeers` DISABLE KEYS */;
/*!40000 ALTER TABLE `sippeers` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `sipusers`
--

DROP TABLE IF EXISTS `sipusers`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `sipusers` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `accountcode` varchar(20) DEFAULT NULL,
  `disallow` varchar(100) DEFAULT 'all',
  `allow` varchar(100) DEFAULT 'g729;ilbc;gsm;ulaw;alaw',
  `allowoverlap` enum('yes','no') DEFAULT 'yes',
  `allowsubscribe` enum('yes','no') DEFAULT 'yes',
  `allowtransfer` varchar(3) DEFAULT NULL,
  `amaflags` varchar(13) DEFAULT NULL,
  `autoframing` varchar(3) DEFAULT NULL,
  `auth` varchar(40) DEFAULT NULL,
  `buggymwi` enum('yes','no') DEFAULT 'no',
  `callgroup` varchar(10) DEFAULT NULL,
  `callerid` varchar(80) DEFAULT NULL,
  `cid_number` varchar(40) DEFAULT NULL,
  `fullname` varchar(40) DEFAULT NULL,
  `call-limit` int(8) DEFAULT '0',
  `callbackextension` varchar(50) DEFAULT '0',
  `callingpres` varchar(80) DEFAULT NULL,
  `canreinvite` char(6) DEFAULT 'yes',
  `context` varchar(80) DEFAULT NULL,
  `defaultip` varchar(15) DEFAULT NULL,
  `dtmfmode` varchar(7) DEFAULT NULL,
  `fromuser` varchar(80) DEFAULT NULL,
  `fromdomain` varchar(80) DEFAULT NULL,
  `fullcontact` varchar(80) DEFAULT NULL,
  `g726nonstandard` enum('yes','no') DEFAULT 'no',
  `host` varchar(31) NOT NULL DEFAULT 'dynamic',
  `insecure` varchar(20) DEFAULT NULL,
  `ipaddr` varchar(45) NOT NULL DEFAULT '',
  `useragent` varchar(80) NOT NULL DEFAULT '',
  `language` char(2) DEFAULT NULL,
  `lastms` varchar(20) DEFAULT NULL,
  `mailbox` varchar(50) DEFAULT NULL,
  `maxcallbitrate` int(8) DEFAULT '384',
  `mohsuggest` varchar(80) DEFAULT NULL,
  `md5secret` varchar(80) DEFAULT NULL,
  `musiconhold` varchar(100) DEFAULT NULL,
  `nat` varchar(15) NOT NULL DEFAULT 'no',
  `name` varchar(254) NOT NULL DEFAULT '',
  `outboundproxy` varchar(80) DEFAULT NULL,
  `deny` varchar(95) DEFAULT NULL,
  `permit` varchar(95) DEFAULT NULL,
  `pickupgroup` varchar(10) DEFAULT NULL,
  `port` varchar(5) NOT NULL DEFAULT '',
  `progressinband` enum('yes','no','never') DEFAULT 'no',
  `promiscredir` enum('yes','no') DEFAULT 'no',
  `qualify` char(3) DEFAULT NULL,
  `regexten` varchar(80) NOT NULL DEFAULT '',
  `regseconds` int(11) NOT NULL DEFAULT '0',
  `rfc2833compensate` enum('yes','no') DEFAULT 'no',
  `rtptimeout` char(3) DEFAULT NULL,
  `rtpholdtimeout` char(3) DEFAULT NULL,
  `secret` varchar(80) DEFAULT NULL,
  `sendrpid` enum('yes','no') DEFAULT 'yes',
  `setvar` varchar(100) NOT NULL DEFAULT '',
  `subscribecontext` varchar(80) DEFAULT NULL,
  `subscribemwi` varchar(3) DEFAULT NULL,
  `t38pt_udptl` enum('yes','no') DEFAULT 'no',
  `trustrpid` enum('yes','no') DEFAULT 'no',
  `type` varchar(6) NOT NULL DEFAULT 'friend',
  `useclientcode` enum('yes','no') DEFAULT 'no',
  `username` varchar(80) NOT NULL DEFAULT '',
  `usereqphone` varchar(3) NOT NULL DEFAULT 'no',
  `videosupport` enum('yes','no') DEFAULT 'yes',
  `vmexten` varchar(80) DEFAULT NULL,
  `defaultuser` varchar(50) DEFAULT NULL,
  `regserver` varchar(50) DEFAULT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `name_UNIQUE` (`name`),
  KEY `name_2` (`name`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `sipusers`
--

LOCK TABLES `sipusers` WRITE;
/*!40000 ALTER TABLE `sipusers` DISABLE KEYS */;
/*!40000 ALTER TABLE `sipusers` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `sms_templates`
--

DROP TABLE IF EXISTS `sms_templates`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `sms_templates` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(254) NOT NULL DEFAULT '0',
  `message` text,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `sms_templates`
--

LOCK TABLES `sms_templates` WRITE;
/*!40000 ALTER TABLE `sms_templates` DISABLE KEYS */;
/*!40000 ALTER TABLE `sms_templates` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `store_cats`
--

DROP TABLE IF EXISTS `store_cats`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `store_cats` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(254) NOT NULL,
  `parent` int(11) DEFAULT NULL,
  `position` int(11) DEFAULT '0',
  `enable` tinyint(1) DEFAULT '1',
  `store_id` int(11) DEFAULT NULL,
  `ico` varchar(50) DEFAULT 'Open_16x16.png',
  PRIMARY KEY (`id`),
  KEY `fk_store_cats_store_cats1_idx` (`parent`),
  KEY `fk_store_cats_stores1_idx` (`store_id`),
  CONSTRAINT `fk_store_cats_store_cats1` FOREIGN KEY (`parent`) REFERENCES `store_cats` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT `fk_store_cats_stores1` FOREIGN KEY (`store_id`) REFERENCES `stores` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `store_cats`
--

LOCK TABLES `store_cats` WRITE;
/*!40000 ALTER TABLE `store_cats` DISABLE KEYS */;
/*!40000 ALTER TABLE `store_cats` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `store_fields`
--

DROP TABLE IF EXISTS `store_fields`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `store_fields` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(254) DEFAULT NULL,
  `store_sub_type` int(11) NOT NULL,
  `field_type` int(11) DEFAULT '0' COMMENT '1 текст\n2 выпадающий список',
  `field_params` text COMMENT 'параметры поля через запятую\\nДля типов поля, которые требуют параметры',
  PRIMARY KEY (`id`),
  KEY `fk_store_fields_store_types1_idx` (`store_sub_type`),
  CONSTRAINT `fk_store_fields_store_types1` FOREIGN KEY (`store_sub_type`) REFERENCES `store_sub_types` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `store_fields`
--

LOCK TABLES `store_fields` WRITE;
/*!40000 ALTER TABLE `store_fields` DISABLE KEYS */;
/*!40000 ALTER TABLE `store_fields` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `store_int_reserve`
--

DROP TABLE IF EXISTS `store_int_reserve`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `store_int_reserve` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `item_id` int(11) NOT NULL,
  `name` varchar(254) DEFAULT NULL,
  `count` int(11) NOT NULL,
  `created` datetime DEFAULT NULL,
  `from_user` int(11) NOT NULL,
  `to_user` int(11) NOT NULL,
  `notes` text NOT NULL,
  `state` int(11) NOT NULL COMMENT '0 ожидание\n1 у мастера\n2 установлена в ремонт\n3 Товар установлен, устройство выдано\n4 архив\n',
  `repair_id` int(11) DEFAULT NULL,
  `work_id` int(11) DEFAULT NULL,
  `price` decimal(19,4) NOT NULL,
  `sn` varchar(254) DEFAULT NULL,
  `warranty` int(11) NOT NULL DEFAULT '0',
  `r_lock` tinyint(1) NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`),
  KEY `fk_store_int_reserve_store_items1_idx` (`item_id`),
  KEY `fk_store_int_reserve_users1_idx` (`to_user`),
  KEY `fk_store_int_reserve_users2_idx` (`from_user`),
  KEY `FK_store_int_reserve_workshop` (`repair_id`),
  KEY `FK_store_int_reserve_works` (`work_id`),
  CONSTRAINT `FK_store_int_reserve_works` FOREIGN KEY (`work_id`) REFERENCES `works` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT `FK_store_int_reserve_workshop` FOREIGN KEY (`repair_id`) REFERENCES `workshop` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT `fk_store_int_reserve_store_items1` FOREIGN KEY (`item_id`) REFERENCES `store_items` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT `fk_store_int_reserve_users1` FOREIGN KEY (`to_user`) REFERENCES `users` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT `fk_store_int_reserve_users2` FOREIGN KEY (`from_user`) REFERENCES `users` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION
) ENGINE=InnoDB AUTO_INCREMENT=23 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `store_int_reserve`
--

LOCK TABLES `store_int_reserve` WRITE;
/*!40000 ALTER TABLE `store_int_reserve` DISABLE KEYS */;
INSERT INTO `store_int_reserve` VALUES (1,9,'L78L05ACZ',2,NULL,1,1,'Автовыдача 1ед. товара сотруднику test для ремонта №1',1,5,NULL,17.0000,NULL,93,0),(2,15,'Кабель FFC/FPC, 24 конт., шаг 0,5мм, тип A, L=100мм',1,NULL,1,1,'',1,NULL,NULL,98.0000,NULL,0,0),(3,45,'Процессор Intel, BGA1449, Core i3-1115G4 SRK08',1,NULL,1,1,'',1,NULL,NULL,12116.0000,NULL,0,0),(4,29,'NX1501-TF',1,NULL,1,1,'',1,NULL,NULL,5.0000,NULL,0,0);
/*!40000 ALTER TABLE `store_int_reserve` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `store_items`
--

DROP TABLE IF EXISTS `store_items`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `store_items` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `Hidden` tinyint(1) NOT NULL DEFAULT '0',
  `articul` int(11) NOT NULL,
  `dealer` int(11) NOT NULL,
  `is_realization` tinyint(1) NOT NULL DEFAULT '0',
  `dealer_lock` int(11) NOT NULL DEFAULT '0',
  `name` varchar(254) NOT NULL,
  `state` int(11) NOT NULL COMMENT '1 = новое\n2 бу',
  `category` int(11) NOT NULL,
  `store` int(11) NOT NULL,
  `created` datetime DEFAULT NULL,
  `updated` datetime DEFAULT NULL,
  `count` int(11) NOT NULL DEFAULT '0',
  `reserved` int(11) NOT NULL DEFAULT '0',
  `units` int(11) NOT NULL DEFAULT '0',
  `box` int(11) DEFAULT NULL,
  `box_name` varchar(254) DEFAULT NULL,
  `price_option` int(11) NOT NULL DEFAULT '1',
  `currency_rate` decimal(19,4) NOT NULL DEFAULT '0.0000',
  `in_price` decimal(19,4) NOT NULL DEFAULT '0.0000',
  `price` decimal(19,4) NOT NULL DEFAULT '0.0000' COMMENT 'для СЦ',
  `price2` decimal(19,4) NOT NULL DEFAULT '0.0000' COMMENT 'Розница',
  `price3` decimal(19,4) NOT NULL DEFAULT '0.0000' COMMENT 'Опт',
  `price4` decimal(19,4) NOT NULL DEFAULT '0.0000' COMMENT 'Опт2',
  `price5` decimal(19,4) NOT NULL DEFAULT '0.0000' COMMENT 'Опт3',
  `document` int(11) DEFAULT NULL,
  `part_request` int(11) DEFAULT NULL,
  `shop_title` varchar(254) DEFAULT NULL COMMENT 'Описание для интернет магазина',
  `shop_description` text COMMENT 'Описание для интернет магазина',
  `SN` varchar(254) DEFAULT NULL COMMENT 'серийный номер',
  `PN` varchar(254) DEFAULT NULL COMMENT 'партномер',
  `description` text,
  `shop_enable` tinyint(1) DEFAULT NULL,
  `int_barcode` varchar(254) DEFAULT NULL,
  `ext_barcode` varchar(254) DEFAULT NULL,
  `in_count` int(11) NOT NULL DEFAULT '0',
  `in_summ` decimal(19,4) NOT NULL DEFAULT '0.0000',
  `notes` varchar(5000) DEFAULT NULL,
  `img1` int(11) DEFAULT NULL,
  `img2` int(11) DEFAULT NULL,
  `img3` int(11) DEFAULT NULL,
  `img4` int(11) DEFAULT NULL,
  `img5` int(11) DEFAULT NULL,
  `minimum_in_stock` int(11) NOT NULL DEFAULT '0',
  `sold` int(11) NOT NULL DEFAULT '0',
  `return_percent` int(11) NOT NULL DEFAULT '0' COMMENT 'процент с прибыли отдаваемый реализатору',
  `warranty` int(11) NOT NULL DEFAULT '0',
  `warranty_dealer` int(11) NOT NULL DEFAULT '0',
  `not_for_sale` tinyint(1) NOT NULL DEFAULT '0',
  `st_state` int(11) NOT NULL DEFAULT '0',
  `st_notes` varchar(254) DEFAULT NULL,
  `ge_highlight` tinyint(1) NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`),
  KEY `fk_store_items_store_cats1_idx` (`category`),
  KEY `fk_store_items_stores1_idx` (`store`),
  KEY `fk_store_items_boxes1_idx` (`box`),
  KEY `fk_store_items_docs1_idx` (`document`),
  KEY `fk_store_items_items_state1_idx` (`state`),
  KEY `fk_store_items_clients1_idx` (`dealer`),
  KEY `articul` (`articul`),
  KEY `name` (`name`),
  KEY `int_barcode` (`int_barcode`),
  KEY `SN` (`SN`),
  KEY `PN` (`PN`),
  KEY `ext_barcode` (`ext_barcode`),
  KEY `FK_store_items_parts_request` (`part_request`),
  KEY `notes` (`notes`(255)) USING BTREE,
  CONSTRAINT `FK_store_items_parts_request` FOREIGN KEY (`part_request`) REFERENCES `parts_request` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT `fk_store_items_boxes1` FOREIGN KEY (`box`) REFERENCES `boxes` (`id`) ON DELETE SET NULL ON UPDATE NO ACTION,
  CONSTRAINT `fk_store_items_clients1` FOREIGN KEY (`dealer`) REFERENCES `clients` (`id`) ON DELETE CASCADE ON UPDATE NO ACTION,
  CONSTRAINT `fk_store_items_docs1` FOREIGN KEY (`document`) REFERENCES `docs` (`id`) ON DELETE CASCADE ON UPDATE NO ACTION,
  CONSTRAINT `fk_store_items_items_state1` FOREIGN KEY (`state`) REFERENCES `items_state` (`id`) ON DELETE CASCADE ON UPDATE NO ACTION,
  CONSTRAINT `fk_store_items_store_cats1` FOREIGN KEY (`category`) REFERENCES `store_cats` (`id`) ON DELETE CASCADE ON UPDATE NO ACTION,
  CONSTRAINT `fk_store_items_stores1` FOREIGN KEY (`store`) REFERENCES `stores` (`id`) ON DELETE CASCADE ON UPDATE NO ACTION
) ENGINE=InnoDB AUTO_INCREMENT=51 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `store_items`
--

LOCK TABLES `store_items` WRITE;
/*!40000 ALTER TABLE `store_items` DISABLE KEYS */;
INSERT INTO `store_items` VALUES (1,0,360,1952,1,0,'216-0707007',1,1039,1,'2016-01-09 15:07:25','2023-11-06 23:58:44',2,0,0,10750,'MAS-KP10/A1',1,25.5400,1420.4995,1847.0000,1847.0000,1847.0000,1847.0000,1847.0000,391,NULL,NULL,NULL,'','','',NULL,'011000060262',NULL,2,777.4400,'',NULL,NULL,NULL,NULL,NULL,0,0,100,0,0,0,0,NULL,0),(2,0,361,1952,1,0,'216-0749001',1,1039,1,'2019-11-30 15:00:01','2023-11-06 23:58:44',1,0,0,10755,'MAS-KP10/A5',1,24.0964,1011.4087,1267.0000,1418.0000,1367.0000,1336.0000,1267.0000,391,NULL,NULL,NULL,NULL,'','',NULL,'011000216922',NULL,1,261.1300,'',NULL,NULL,NULL,NULL,NULL,0,0,100,186,0,0,0,NULL,0),(3,0,380,1952,1,0,'218-0755042',1,1039,1,'2021-02-25 16:08:09','2024-02-14 09:30:48',1,0,0,10757,'MAS-KP10/B2',1,28.0112,1139.4098,1596.0000,1596.0000,1483.0000,1426.0000,1369.0000,391,NULL,NULL,NULL,NULL,'','',NULL,'011000255732',NULL,2,683.9400,'',NULL,NULL,NULL,NULL,NULL,0,1,100,186,0,0,0,NULL,0),(4,0,396,1952,1,0,'CG82NM10 SLGXX',1,1040,1,'2021-02-25 16:08:09','2023-11-06 23:58:44',3,0,0,10769,'MAS-KP10/D3',1,28.0112,759.6066,1066.0000,1066.0000,990.0000,950.0000,913.0000,391,NULL,NULL,NULL,NULL,'','',NULL,'011000255622',NULL,3,683.9400,'',NULL,NULL,NULL,NULL,NULL,0,0,100,186,0,0,0,NULL,0),(5,0,470,1952,1,0,'ISL95831HRTZ',1,1020,1,'2019-11-30 15:00:01','2023-11-06 23:58:44',2,0,0,10213,'IC/A2',1,24.0964,50.5840,240.0000,240.0000,240.0000,240.0000,240.0000,391,NULL,NULL,NULL,NULL,'','',NULL,'011000217032',NULL,2,26.1200,'',NULL,NULL,NULL,NULL,NULL,0,0,100,31,0,0,0,NULL,0),(6,0,514,1952,1,0,'TPS51620',1,1020,1,'2015-03-08 15:26:41','2023-11-06 23:58:44',1,0,0,10440,'IC/D9',1,27.5800,0.0000,700.0000,700.0000,700.0000,700.0000,700.0000,391,NULL,NULL,NULL,'','','',NULL,'011000008742',NULL,1,0.0000,'',NULL,NULL,NULL,NULL,NULL,0,0,100,0,0,0,0,NULL,0),(7,0,540,1952,1,0,'LD1117S33TR',1,1023,1,'2015-03-16 13:08:16','2023-11-06 23:58:44',3,0,0,10335,'IC/C21',1,25.3900,20.5483,27.0000,27.0000,27.0000,27.0000,27.0000,391,NULL,NULL,NULL,'','','',NULL,'011000013732',NULL,3,16.7700,'',NULL,NULL,NULL,NULL,NULL,0,0,100,0,0,0,0,NULL,0),(8,0,542,1952,1,0,'LM1117-ADJ',2,1023,1,'2015-03-16 13:05:45','2023-11-06 23:58:44',3,0,0,10276,'IC/B22',1,25.3900,10.2558,13.0000,13.0000,13.0000,13.0000,13.0000,391,NULL,NULL,NULL,'','','',NULL,'011000012942',NULL,3,8.3700,'',NULL,NULL,NULL,NULL,NULL,0,0,100,0,0,0,0,NULL,0),(9,0,666,1952,1,0,'L78L05ACZ',1,1023,1,'2015-03-16 13:08:16','2023-11-06 23:58:44',10,0,0,10212,'IC/A19',1,25.3900,13.0494,17.0000,17.0000,17.0000,17.0000,17.0000,391,NULL,NULL,NULL,'','','',NULL,'011000013672',NULL,10,35.5000,'',NULL,NULL,NULL,NULL,NULL,0,0,100,0,0,0,0,NULL,0),(10,0,769,1952,1,0,'Кнопка тактовая SWT-39, 3,7х6мм, h=2,5мм, SMT',1,1089,1,'2016-02-25 12:23:34','2023-11-06 23:58:44',1,0,0,10772,'SW/A3',1,27.4100,2.7921,11.0000,11.0000,11.0000,11.0000,11.0000,391,NULL,NULL,NULL,'','','',NULL,'011000064092',NULL,1,0.8200,'',NULL,NULL,NULL,NULL,NULL,0,0,100,0,0,0,0,NULL,0),(11,0,782,1952,1,0,'BC856B',1,1054,1,'2021-06-16 08:52:52','2023-11-06 23:58:44',30,0,0,10017,'BJT/C4',1,27.1003,1.9286,7.0000,7.0000,7.0000,3.0000,3.0000,391,NULL,NULL,NULL,NULL,'','',NULL,'011000261992',NULL,30,16.8000,'',NULL,NULL,NULL,NULL,NULL,0,0,100,31,0,0,0,NULL,0),(12,0,785,1952,1,0,'OB2263MP',1,1021,1,'2017-01-12 08:52:16','2023-11-06 23:58:44',2,0,0,10252,'IC/A55',1,28.5200,36.3891,182.0000,182.0000,182.0000,182.0000,182.0000,391,NULL,NULL,NULL,'','','',NULL,'011000101062',NULL,2,22.2400,'',NULL,NULL,NULL,NULL,NULL,0,0,100,0,0,0,0,NULL,0),(13,0,845,1952,1,0,'TL331IDBVR',1,1028,1,'2016-02-25 12:23:34','2023-11-06 23:58:44',9,0,0,10544,'IC/F5',1,27.4100,24.2777,49.0000,49.0000,49.0000,49.0000,49.0000,391,NULL,NULL,NULL,'','','',NULL,'011000064102',NULL,9,64.1700,'',NULL,NULL,NULL,NULL,NULL,0,0,100,0,0,0,0,NULL,0),(14,0,869,1952,1,0,'Кабель FFC/FPC, 6 конт., шаг 0,5мм, тип D, L=100мм',1,1102,1,'2016-04-18 16:12:28','2023-11-06 23:58:44',3,0,0,10178,'FFC/FPC 0.5',1,25.7900,19.6139,98.0000,98.0000,78.0000,51.0000,37.0000,391,NULL,NULL,NULL,'','','',NULL,'011000069932',NULL,3,16.2600,'',NULL,NULL,NULL,NULL,NULL,0,0,100,0,0,0,0,NULL,0),(15,0,928,1952,1,0,'Кабель FFC/FPC, 24 конт., шаг 0,5мм, тип A, L=100мм',1,1102,1,'2016-04-18 16:12:28','2023-11-06 23:58:44',4,0,0,10178,'FFC/FPC 0.5',1,25.7900,19.6139,98.0000,98.0000,78.0000,51.0000,37.0000,391,NULL,NULL,NULL,'','','',NULL,'011000070522',NULL,4,21.6800,'',NULL,NULL,NULL,NULL,NULL,0,0,100,0,0,0,0,NULL,0),(16,0,938,1952,1,0,'Кабель FFC/FPC, 25 конт., шаг 0,5мм, тип A, L=200мм',1,1102,1,'2016-04-18 16:12:28','2023-11-06 23:58:44',5,0,0,10178,'FFC/FPC 0.5',1,25.7900,19.6139,98.0000,98.0000,78.0000,51.0000,37.0000,391,NULL,NULL,NULL,'','','',NULL,'011000070622',NULL,5,27.1000,'',NULL,NULL,NULL,NULL,NULL,0,0,100,0,0,0,0,NULL,0),(17,0,958,1952,1,0,'Кабель FFC/FPC, 32 конт., шаг 0,5мм, тип A, L=100мм',1,1102,1,'2016-04-18 16:12:28','2023-11-06 23:58:44',5,0,0,10178,'FFC/FPC 0.5',1,25.7900,19.6139,98.0000,98.0000,78.0000,51.0000,37.0000,391,NULL,NULL,NULL,'','','',NULL,'011000070822',NULL,5,27.1000,'',NULL,NULL,NULL,NULL,NULL,0,0,100,0,0,0,0,NULL,0),(18,0,978,1952,1,0,'Кабель FFC/FPC, 5 конт., шаг 1,0мм, тип A, L=150мм',1,1102,1,'2016-04-18 16:12:28','2023-11-06 23:58:44',5,0,0,10179,'FFC/FPC 1.0',1,25.7900,19.6139,98.0000,98.0000,78.0000,51.0000,37.0000,391,NULL,NULL,NULL,'','','',NULL,'011000071022',NULL,5,27.1000,'',NULL,NULL,NULL,NULL,NULL,0,0,100,0,0,0,0,NULL,0),(19,0,1029,1952,1,0,'Кабель FFC/FPC, 18 конт., шаг 1,0мм, тип D, L=200мм',1,1102,1,'2016-04-18 16:12:28','2023-11-06 23:58:44',5,0,0,10179,'FFC/FPC 1.0',1,25.7900,19.6139,98.0000,98.0000,78.0000,51.0000,37.0000,391,NULL,NULL,NULL,'','','',NULL,'011000071532',NULL,5,27.1000,'',NULL,NULL,NULL,NULL,NULL,0,0,100,0,0,0,0,NULL,0),(20,0,1038,1952,1,0,'Кабель FFC/FPC, 22 конт., шаг 1,0мм, тип A, L=150мм',1,1102,1,'2016-04-18 16:12:28','2023-11-06 23:58:44',5,0,0,10179,'FFC/FPC 1.0',1,25.7900,19.6139,98.0000,98.0000,78.0000,51.0000,37.0000,391,NULL,NULL,NULL,'','','',NULL,'011000071622',NULL,5,27.1000,'',NULL,NULL,NULL,NULL,NULL,0,0,100,0,0,0,0,NULL,0),(21,0,1061,1952,1,0,'Кабель FFC/FPC, 28 конт., шаг 1,0мм, тип D, L=100мм',1,1102,1,'2016-04-18 16:12:28','2023-11-06 23:58:44',5,0,0,10179,'FFC/FPC 1.0',1,25.7900,19.6139,98.0000,98.0000,78.0000,51.0000,37.0000,391,NULL,NULL,NULL,'','','',NULL,'011000071852',NULL,5,27.1000,'',NULL,NULL,NULL,NULL,NULL,0,0,100,0,0,0,0,NULL,0),(22,0,1072,1952,1,0,'Кабель FFC/FPC, 32 конт., шаг 1,0мм, тип A, L=100мм',1,1102,1,'2016-04-18 16:12:28','2023-11-06 23:58:44',5,0,0,10179,'FFC/FPC 1.0',1,25.7900,19.6139,98.0000,98.0000,78.0000,51.0000,37.0000,391,NULL,NULL,NULL,'','','',NULL,'011000071962',NULL,5,27.1000,'',NULL,NULL,NULL,NULL,NULL,0,0,100,0,0,0,0,NULL,0),(23,0,1115,1952,1,0,'Гнездо FFC/FPC, 14 конт., шаг 1,0мм, ZIF, slide lock, угловой, SMT, контакты сверху',1,1096,1,'2016-04-18 16:12:28','2023-11-06 23:58:44',5,0,0,10126,'FFC/B10',1,25.7900,19.6139,98.0000,98.0000,98.0000,98.0000,98.0000,391,NULL,NULL,NULL,'','','',NULL,'011000072392',NULL,5,27.1000,'',NULL,NULL,NULL,NULL,NULL,0,0,100,0,0,0,0,NULL,0),(24,0,1161,1952,1,0,'Кнопка тактовая TTS5-015, 5x5мм, h=2,5мм',1,1089,1,'2016-05-31 14:10:33','2023-11-06 23:58:44',9,0,0,10798,'SW/D4',1,25.2300,5.5858,28.0000,28.0000,28.0000,28.0000,28.0000,391,NULL,NULL,NULL,'','','',NULL,'011000076462',NULL,9,13.5900,'',NULL,NULL,NULL,NULL,NULL,0,0,100,0,0,0,0,NULL,0),(25,0,1256,1952,1,0,'IT8728F DXA GB',1,1045,1,'2016-12-17 13:00:04','2023-11-06 23:58:44',1,0,0,10806,'TRESTON/B8',1,27.0000,375.1902,563.0000,563.0000,563.0000,563.0000,563.0000,391,NULL,NULL,NULL,'','','',NULL,'011000098862',NULL,1,108.5400,'',NULL,NULL,NULL,NULL,NULL,0,0,100,0,0,0,0,NULL,0),(26,0,1266,1952,1,0,'2N5401',1,1054,1,'2017-02-09 15:52:02','2023-11-06 23:58:44',10,0,0,10024,'BJT/D5',1,27.5500,2.8118,17.0000,17.0000,17.0000,17.0000,17.0000,391,NULL,NULL,NULL,'','','',NULL,'011000103602',NULL,10,8.3000,'',NULL,NULL,NULL,NULL,NULL,0,0,100,0,0,0,0,NULL,0),(27,0,1272,1952,1,0,'BQ24296',1,1019,1,'2018-05-18 15:52:57','2023-11-06 23:58:44',2,0,0,10513,'IC/F21',1,26.1900,103.5939,139.0000,416.0000,312.0000,208.0000,139.0000,391,NULL,NULL,NULL,'','','',NULL,'011000159532',NULL,2,58.1400,'',NULL,NULL,NULL,NULL,NULL,0,0,100,0,0,0,0,NULL,0),(28,0,1275,1952,1,0,'RT8871AZQW',1,1020,1,'2017-03-02 12:34:37','2023-11-06 23:58:44',3,0,0,10508,'IC/F17',1,27.2700,91.4465,183.0000,183.0000,183.0000,183.0000,183.0000,391,NULL,NULL,NULL,'','','',NULL,'011000106572',NULL,3,80.1600,'',NULL,NULL,NULL,NULL,NULL,0,0,100,0,0,0,0,NULL,0),(29,0,1311,1952,1,0,'NX1501-TF',1,1097,1,'2017-07-24 09:42:10','2023-11-06 23:58:44',43,0,0,10726,'LVCON/B8',1,25.9800,0.9340,5.0000,5.0000,5.0000,5.0000,5.0000,391,NULL,NULL,NULL,'','','',NULL,'011000122922',NULL,43,11.1800,'',NULL,NULL,NULL,NULL,NULL,0,0,100,0,0,0,0,NULL,0),(30,0,1328,1952,1,0,'BQ24296M',1,1019,1,'2019-02-05 16:27:26','2023-11-06 23:58:44',4,0,0,10513,'IC/F21',1,28.0100,77.4690,280.0000,280.0000,280.0000,280.0000,280.0000,391,NULL,NULL,NULL,'','','',NULL,'011000194382',NULL,4,93.0000,'',NULL,NULL,NULL,NULL,NULL,0,0,100,0,0,0,0,NULL,0),(31,0,1341,1952,1,0,'Процессор Intel, BGA1023, Core i5-2410M SR04G',1,1040,1,'2017-12-26 22:59:32','2023-11-06 23:58:44',1,0,0,10767,'MAS-KP10/D1',1,27.9400,3750.0000,4876.0000,4876.0000,4876.0000,4876.0000,4876.0000,391,NULL,NULL,NULL,'','','',NULL,'011000142952',NULL,1,1122.9100,'',NULL,NULL,NULL,NULL,NULL,0,0,100,0,0,0,0,NULL,0),(32,0,1368,1952,1,0,'ALC1301',1,1025,1,'2019-02-05 16:27:26','2023-11-06 23:58:44',5,0,0,10634,'IC/H23',1,28.0100,194.1223,727.0000,727.0000,727.0000,727.0000,727.0000,391,NULL,NULL,NULL,'','','',NULL,'011000194302',NULL,5,291.3000,'',NULL,NULL,NULL,NULL,NULL,0,0,100,0,0,0,0,NULL,0),(33,0,1417,1952,1,0,'Предохранитель SMD1808 5А',1,1087,1,'2019-01-15 17:06:59','2025-04-23 07:06:29',10,0,0,10185,'FuseSMD/A5',1,28.2500,14.9327,56.0000,56.0000,56.0000,56.0000,56.0000,391,NULL,NULL,NULL,'','','',NULL,'011000188812',NULL,10,45.2000,'',NULL,NULL,NULL,NULL,NULL,0,0,100,0,0,0,0,NULL,0),(34,0,1425,1952,1,0,'Кнопка тактовая, SMT, 5x5мм, h=0,8мм, без толкателя',1,1089,1,'2019-01-18 10:43:36','2023-11-06 23:58:44',90,0,0,10778,'SW/A9',1,27.7800,0.9407,6.0000,6.0000,6.0000,6.0000,6.0000,391,NULL,NULL,NULL,'','','',NULL,'011000189572',NULL,90,25.2000,'',NULL,NULL,NULL,NULL,NULL,0,0,100,0,0,0,0,NULL,0),(35,0,1431,1952,1,0,'OB2273MP',1,1021,1,'2019-01-30 15:33:29','2024-02-20 14:47:23',2,0,0,10529,'IC/F36',1,27.9300,54.1339,85.0000,85.0000,85.0000,85.0000,85.0000,391,NULL,NULL,NULL,'','','',NULL,'011000191292',NULL,3,48.6000,'',NULL,NULL,NULL,NULL,NULL,0,1,100,0,0,0,0,NULL,0),(36,0,1477,1952,1,0,'Гнездо FFC/FPC, 22 конт., шаг 1,0мм, ZIF, slide lock, угловой, SMT, контакты снизу',1,1096,1,'2019-02-22 09:44:11','2023-11-06 23:58:44',10,0,0,10114,'FFC/A15',1,27.3200,18.6525,109.0000,109.0000,109.0000,109.0000,109.0000,391,NULL,NULL,NULL,'','','',NULL,'011000197402',NULL,10,54.6000,'',NULL,NULL,NULL,NULL,NULL,0,0,100,0,0,0,0,NULL,0),(37,0,1540,1952,1,0,'Предохранитель SMD0402 1A',1,1087,1,'2019-09-16 10:46:14','2023-11-06 23:58:44',6,0,0,10181,'FuseSMD/A1',1,25.0000,9.5197,41.0000,60.0000,52.0000,45.0000,41.0000,391,NULL,NULL,NULL,NULL,NULL,'',NULL,'011000211322',NULL,6,15.3000,'',NULL,NULL,NULL,NULL,NULL,0,0,100,0,0,0,0,NULL,0),(38,0,1637,1952,1,0,'Конденсатор керамический 100нФ*16В, 10%, X7R, 0402',1,1077,1,'2020-07-28 10:31:17','2023-11-06 23:58:44',73,0,0,10036,'Cap/B2',1,8.0000,0.3500,1.0000,1.0000,1.0000,1.0000,1.0000,391,NULL,NULL,NULL,NULL,'MNC0402X7R104K16TR','',NULL,'011000240012',NULL,73,2.1900,'',NULL,NULL,NULL,NULL,NULL,0,0,100,31,0,0,0,NULL,0),(39,0,1667,1952,1,0,'Сверло по металлу d=1,9мм, Р6М5',1,1113,1,'2020-09-16 10:54:31','2023-11-06 23:58:44',4,0,0,10802,'ToolBoxII',1,28.0112,18.4587,30.0000,30.0000,26.0000,26.0000,23.0000,391,NULL,NULL,NULL,NULL,'Ворон: 033047','',NULL,'011000244972',NULL,4,22.1600,'',NULL,NULL,NULL,NULL,NULL,0,0,100,0,0,0,0,NULL,0),(40,0,1677,1952,1,0,'NCP302045MN',1,1057,1,'2020-10-13 19:28:31','2023-11-06 23:58:44',10,0,0,10548,'IC/F53',1,28.4091,120.4683,355.0000,355.0000,332.0000,296.0000,237.0000,391,NULL,NULL,NULL,NULL,'','',NULL,'011000247132',NULL,10,366.7000,'',NULL,NULL,NULL,NULL,NULL,0,0,100,31,0,0,0,NULL,0),(41,0,1686,1952,1,0,'Сверло по металлу d=1,2мм, Р6М5',1,1113,1,'2020-11-20 18:55:06','2023-11-06 23:58:44',3,0,0,10802,'ToolBoxII',1,28.5714,32.1433,65.0000,65.0000,59.0000,52.0000,39.0000,391,NULL,NULL,NULL,NULL,'','',NULL,'011000250052',NULL,3,29.5200,'',NULL,NULL,NULL,NULL,NULL,0,0,100,0,0,0,0,NULL,0),(42,0,1821,1952,1,0,'Гнездо FFC/FPC, 25 конт., шаг 0,4мм, ZIF, flip lock, угловой, SMT, контакты снизу',1,1096,1,'2022-01-26 10:40:34','2023-11-06 23:58:44',1,0,0,10905,'Сортировка',1,28.4900,0.0000,324.0000,324.0000,324.0000,324.0000,324.0000,391,NULL,NULL,NULL,NULL,'','',NULL,'011000274792',NULL,1,0.0000,'',NULL,NULL,NULL,NULL,NULL,0,0,100,0,0,0,0,NULL,0),(43,0,1832,1952,1,0,'Гнездо FFC/FPC, 6 конт., шаг 1,0мм, ZIF, flip lock, угловой, SMT, контакты снизу',2,1096,1,'2022-02-15 08:25:47','2023-11-06 23:58:44',1,0,0,10124,'FFC/A8',1,28.4900,0.0000,318.0000,318.0000,285.0000,252.0000,197.0000,391,NULL,NULL,NULL,NULL,'','',NULL,'011000275642',NULL,1,0.0000,'',NULL,NULL,NULL,NULL,NULL,0,0,100,0,0,0,0,NULL,0),(44,0,1834,1952,1,0,'Гнездо FFC/FPC, 16 конт., шаг 0,5мм, ZIF, flip lock, угловой, SMT, контакты снизу',2,1096,1,'2022-02-15 08:25:47','2023-11-06 23:58:44',1,0,0,10130,'FFC/B14',1,28.4900,0.0000,318.0000,318.0000,285.0000,252.0000,197.0000,391,NULL,NULL,NULL,NULL,'','',NULL,'011000275672',NULL,1,0.0000,'',NULL,NULL,NULL,NULL,NULL,0,0,100,0,0,0,0,NULL,0),(45,0,1848,1952,1,0,'Процессор Intel, BGA1449, Core i3-1115G4 SRK08',1,1040,1,'2022-07-27 17:01:30','2023-11-06 23:58:44',1,0,0,10762,'MAS-KP10/C2',1,39.5793,10096.0000,12116.0000,12116.0000,11915.0000,11611.0000,11611.0000,391,NULL,NULL,NULL,NULL,'','',NULL,'011000279512',NULL,1,4281.6400,'',NULL,NULL,NULL,NULL,NULL,0,0,100,186,0,0,0,NULL,0),(46,0,1930,1952,1,0,'GP28S50G',1,1055,1,'2023-07-04 10:06:51','2023-11-06 23:58:44',4,0,0,10912,'FETN/C14',1,80.5300,65.1997,220.0000,220.0000,208.0000,192.0000,187.0000,391,NULL,NULL,NULL,NULL,'','',NULL,'011000281972',NULL,4,225.0400,'',NULL,NULL,NULL,NULL,NULL,0,0,100,31,0,0,0,NULL,0),(47,0,2019,1952,1,0,'Колпачек рычага геймпада PS5, серый',1,1130,1,'2023-12-07 00:41:41',NULL,4,0,0,NULL,'',1,96.7100,29.6900,75.0000,75.0000,69.0000,35.0000,60.0000,415,NULL,NULL,NULL,NULL,NULL,'',NULL,'011000026482',NULL,10,118.7560,'',NULL,NULL,NULL,NULL,NULL,0,6,100,0,0,0,0,NULL,0),(48,0,2059,1952,1,0,'Набор резисторов SMD 0603 1%, 50шт * 170 наименований',1,1065,1,'2023-12-07 00:41:41',NULL,1,0,0,NULL,'',1,96.7100,1015.5400,1524.0000,1524.0000,1422.0000,1321.0000,1219.0000,415,NULL,NULL,NULL,NULL,NULL,'',NULL,'011000026902',NULL,3,1015.5359,'',NULL,NULL,NULL,NULL,NULL,0,2,100,0,0,1,0,NULL,0),(49,0,2085,1952,1,0,'3289F',1,1037,1,'2018-03-19 12:36:55',NULL,8,0,0,10905,'Сортировка',1,26.3500,236.8090,141.0000,141.0000,141.0000,141.0000,141.0000,391,NULL,NULL,NULL,'','','',NULL,'',NULL,8,408.0000,'',NULL,NULL,NULL,NULL,NULL,0,0,100,0,0,0,0,NULL,0),(50,0,365,1952,1,0,'Конденсатор керамический 22мкФ*25В, X7R, 10%, 1206',1,1077,0,'2025-11-20 01:33:14',NULL,100,0,0,NULL,NULL,1,27.9400,9.4700,19.0000,19.0000,19.0000,19.0000,19.0000,391,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,100,947.0000,NULL,NULL,NULL,NULL,NULL,NULL,0,0,100,0,0,0,0,NULL,0);
/*!40000 ALTER TABLE `store_items` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `store_sales`
--

DROP TABLE IF EXISTS `store_sales`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `store_sales` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `dealer` int(11) NOT NULL,
  `item_id` int(11) NOT NULL,
  `document_id` int(11) NOT NULL,
  `customer_id` int(11) DEFAULT NULL,
  `count` int(11) NOT NULL DEFAULT '1',
  `in_price` decimal(19,4) NOT NULL DEFAULT '0.0000',
  `price` decimal(19,4) NOT NULL DEFAULT '0.0000',
  `warranty` int(11) NOT NULL DEFAULT '0',
  `realizator_payed` tinyint(1) NOT NULL DEFAULT '0',
  `is_realization` tinyint(1) NOT NULL DEFAULT '0',
  `dealer_payment` int(11) DEFAULT NULL,
  `return_percent` int(11) NOT NULL DEFAULT '0',
  `user` int(11) NOT NULL,
  `is_cancellation` tinyint(1) NOT NULL DEFAULT '0',
  `cancellation_reason` varchar(254) DEFAULT NULL,
  `sn` varchar(254) DEFAULT NULL,
  `d_category` int(11) DEFAULT NULL,
  PRIMARY KEY (`id`),
  KEY `fk_store_sales_store_items1_idx` (`item_id`),
  KEY `fk_store_sales_docs1_idx` (`document_id`),
  KEY `fk_store_sales_clients1_idx` (`dealer`),
  KEY `fk_store_sales_users1_idx` (`user`),
  KEY `FK_store_sales_dealer_payments` (`dealer_payment`),
  KEY `FK_store_sales_clients` (`customer_id`),
  CONSTRAINT `FK_store_sales_clients` FOREIGN KEY (`customer_id`) REFERENCES `clients` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT `FK_store_sales_dealer_payments` FOREIGN KEY (`dealer_payment`) REFERENCES `dealer_payments` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT `fk_store_sales_clients1` FOREIGN KEY (`dealer`) REFERENCES `clients` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT `fk_store_sales_docs1` FOREIGN KEY (`document_id`) REFERENCES `docs` (`id`) ON DELETE CASCADE ON UPDATE NO ACTION,
  CONSTRAINT `fk_store_sales_store_items1` FOREIGN KEY (`item_id`) REFERENCES `store_items` (`id`) ON DELETE CASCADE ON UPDATE NO ACTION,
  CONSTRAINT `fk_store_sales_users1` FOREIGN KEY (`user`) REFERENCES `users` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION
) ENGINE=InnoDB AUTO_INCREMENT=7 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `store_sales`
--

LOCK TABLES `store_sales` WRITE;
/*!40000 ALTER TABLE `store_sales` DISABLE KEYS */;
INSERT INTO `store_sales` VALUES (1,1952,26,1,864,2,2.8118,17.0000,62,0,1,NULL,100,1,0,NULL,'',NULL),(2,1952,37,1,864,1,9.5197,52.0000,0,0,1,NULL,100,1,0,NULL,NULL,NULL),(3,1952,36,1,864,4,18.6525,109.0000,62,0,1,NULL,100,1,0,NULL,'',NULL),(4,1952,31,2,864,1,3750.0000,4876.0000,93,0,1,NULL,100,1,0,NULL,'',NULL);
/*!40000 ALTER TABLE `store_sales` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `store_sub_types`
--

DROP TABLE IF EXISTS `store_sub_types`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `store_sub_types` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(254) DEFAULT NULL,
  `enable` tinyint(1) DEFAULT '1',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=3 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `store_sub_types`
--

LOCK TABLES `store_sub_types` WRITE;
/*!40000 ALTER TABLE `store_sub_types` DISABLE KEYS */;
INSERT INTO `store_sub_types` VALUES (1,'',1);
/*!40000 ALTER TABLE `store_sub_types` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `store_types`
--

DROP TABLE IF EXISTS `store_types`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `store_types` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(254) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `store_types`
--

LOCK TABLES `store_types` WRITE;
/*!40000 ALTER TABLE `store_types` DISABLE KEYS */;
INSERT INTO `store_types` VALUES (1,'локальный');
/*!40000 ALTER TABLE `store_types` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `stores`
--

DROP TABLE IF EXISTS `stores`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `stores` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(254) NOT NULL,
  `office` int(11) DEFAULT NULL,
  `type` int(11) NOT NULL,
  `description` varchar(254) DEFAULT NULL COMMENT 'Описание для склада (заметки?)',
  `sub_type` int(11) NOT NULL,
  `it_vis_pn` tinyint(1) NOT NULL DEFAULT '1',
  `it_vis_notes` tinyint(1) NOT NULL DEFAULT '1',
  `it_vis_description` tinyint(1) NOT NULL DEFAULT '1',
  `it_vis_sn` tinyint(1) NOT NULL DEFAULT '1',
  `it_vis_barcode` tinyint(1) NOT NULL DEFAULT '1',
  `it_vis_photo` tinyint(1) NOT NULL DEFAULT '1',
  `it_vis_warranty` tinyint(1) NOT NULL DEFAULT '1',
  `it_vis_warranty_dealer` tinyint(1) NOT NULL DEFAULT '1',
  `active` tinyint(1) NOT NULL DEFAULT '1',
  PRIMARY KEY (`id`),
  KEY `fk_stores_store_types1_idx` (`type`),
  KEY `fk_stores_store_sub_types1_idx` (`sub_type`),
  KEY `fk_stores_offices1_idx` (`office`),
  CONSTRAINT `fk_stores_offices1` FOREIGN KEY (`office`) REFERENCES `offices` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT `fk_stores_store_sub_types1` FOREIGN KEY (`sub_type`) REFERENCES `store_sub_types` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT `fk_stores_store_types1` FOREIGN KEY (`type`) REFERENCES `store_types` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `stores`
--

LOCK TABLES `stores` WRITE;
/*!40000 ALTER TABLE `stores` DISABLE KEYS */;
INSERT INTO `stores` VALUES (1,'Склад',1,1,NULL,1,1,1,1,1,1,1,1,1,1);
/*!40000 ALTER TABLE `stores` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `task_employees`
--

DROP TABLE IF EXISTS `task_employees`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `task_employees` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `employee` int(11) NOT NULL,
  `task` int(11) NOT NULL,
  PRIMARY KEY (`id`),
  KEY `FK_task_employees_users` (`employee`),
  KEY `FK_task_employees_tasks` (`task`),
  CONSTRAINT `FK_task_employees_tasks` FOREIGN KEY (`task`) REFERENCES `tasks` (`idt`) ON DELETE CASCADE ON UPDATE NO ACTION,
  CONSTRAINT `FK_task_employees_users` FOREIGN KEY (`employee`) REFERENCES `users` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `task_employees`
--

LOCK TABLES `task_employees` WRITE;
/*!40000 ALTER TABLE `task_employees` DISABLE KEYS */;
/*!40000 ALTER TABLE `task_employees` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `tasks`
--

DROP TABLE IF EXISTS `tasks`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `tasks` (
  `idt` int(11) NOT NULL AUTO_INCREMENT,
  `type` int(11) NOT NULL DEFAULT '0' COMMENT '0 = sms\n1 = заказ запчастей\n2 = подтверждение диагностики\n3 входящий звонок',
  `state` int(11) NOT NULL DEFAULT '1' COMMENT '1 активное\n2 выполнено',
  `title` varchar(254) NOT NULL,
  `text` text,
  `top_user` int(11) NOT NULL,
  `parent_task` int(11) DEFAULT NULL,
  `start_datetime` datetime NOT NULL,
  `created` datetime NOT NULL,
  `end_datetime` datetime DEFAULT NULL,
  `is_timelimit` tinyint(1) NOT NULL DEFAULT '0',
  `item_id` int(11) DEFAULT NULL,
  `repair_id` int(11) DEFAULT NULL,
  `task_author` int(11) DEFAULT NULL,
  `complete_datetime` datetime DEFAULT NULL,
  `request_id` int(11) DEFAULT NULL,
  `part_request` int(11) DEFAULT NULL,
  `doc_id` int(11) DEFAULT NULL,
  `priority` int(11) NOT NULL DEFAULT '1',
  `email` varchar(254) DEFAULT NULL,
  `phone` varchar(254) DEFAULT NULL,
  `m_device` int(11) DEFAULT NULL,
  `m_maker` int(11) DEFAULT NULL,
  `m_match` varchar(254) DEFAULT NULL,
  PRIMARY KEY (`idt`),
  KEY `fk_tasks_users2_idx` (`top_user`),
  KEY `fk_tasks_tasks1_idx` (`parent_task`),
  KEY `fk_tasks_users3_idx` (`task_author`),
  KEY `start_datetime` (`start_datetime`),
  KEY `title` (`title`),
  KEY `state` (`state`),
  KEY `FK_tasks_store_int_reserve` (`request_id`),
  KEY `FK_tasks_parts_request` (`part_request`),
  KEY `type` (`type`),
  KEY `priority` (`priority`),
  KEY `FK_tasks_docs` (`doc_id`),
  CONSTRAINT `FK_tasks_docs` FOREIGN KEY (`doc_id`) REFERENCES `docs` (`id`) ON DELETE SET NULL ON UPDATE CASCADE,
  CONSTRAINT `FK_tasks_parts_request` FOREIGN KEY (`part_request`) REFERENCES `parts_request` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT `FK_tasks_store_int_reserve` FOREIGN KEY (`request_id`) REFERENCES `store_int_reserve` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT `fk_tasks_tasks1` FOREIGN KEY (`parent_task`) REFERENCES `tasks` (`idt`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT `fk_tasks_users3` FOREIGN KEY (`task_author`) REFERENCES `users` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `tasks`
--

LOCK TABLES `tasks` WRITE;
/*!40000 ALTER TABLE `tasks` DISABLE KEYS */;
/*!40000 ALTER TABLE `tasks` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `tech_reports`
--

DROP TABLE IF EXISTS `tech_reports`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `tech_reports` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `num` varchar(50) DEFAULT NULL,
  `created` datetime NOT NULL,
  `user` int(11) NOT NULL,
  `company` int(11) NOT NULL,
  `client` int(11) NOT NULL,
  `device` varchar(255) NOT NULL,
  `inventory_number` varchar(255) DEFAULT NULL,
  `serial_number` varchar(255) DEFAULT 'нет',
  `production_date` date DEFAULT NULL,
  `purchase_date` date DEFAULT NULL,
  `initial_cost` decimal(11,4) DEFAULT NULL,
  `residual_cost` decimal(11,4) DEFAULT NULL,
  `fault` varchar(255) DEFAULT '',
  `diagnostic_result` text,
  `conclusion` text,
  `notes` text,
  PRIMARY KEY (`id`),
  KEY `FK_tech_reports_users` (`user`),
  KEY `FK_tech_reports_companies` (`company`),
  KEY `FK_tech_reports_clients` (`client`),
  CONSTRAINT `FK_tech_reports_clients` FOREIGN KEY (`client`) REFERENCES `clients` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT `FK_tech_reports_companies` FOREIGN KEY (`company`) REFERENCES `companies` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT `FK_tech_reports_users` FOREIGN KEY (`user`) REFERENCES `users` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COMMENT='акты технического состояния';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `tech_reports`
--

LOCK TABLES `tech_reports` WRITE;
/*!40000 ALTER TABLE `tech_reports` DISABLE KEYS */;
/*!40000 ALTER TABLE `tech_reports` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `tel`
--

DROP TABLE IF EXISTS `tel`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `tel` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `phone` varchar(45) NOT NULL DEFAULT '0',
  `phone_clean` varchar(45) NOT NULL DEFAULT '0',
  `mask` int(11) unsigned NOT NULL,
  `customer` int(11) DEFAULT NULL,
  `type` int(11) NOT NULL DEFAULT '0' COMMENT '1 основной',
  `note` varchar(254) DEFAULT NULL COMMENT 'примечание',
  `notify` tinyint(1) NOT NULL DEFAULT '1' COMMENT 'Согласен получать SMS',
  `viber` tinyint(1) NOT NULL DEFAULT '0',
  `telegram` tinyint(1) NOT NULL DEFAULT '0',
  `whatsapp` tinyint(1) NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`),
  KEY `phone` (`phone`),
  KEY `phone_clean` (`phone_clean`),
  KEY `customer` (`customer`),
  KEY `type` (`type`)
) ENGINE=InnoDB AUTO_INCREMENT=3 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `tel`
--

LOCK TABLES `tel` WRITE;
/*!40000 ALTER TABLE `tel` DISABLE KEYS */;
INSERT INTO `tel` VALUES (1,'+7123-872 87 21','71238728721',0,1952,3,NULL,1,0,0,0),(2,'+7123-697 96 43','71236979643',0,111,3,NULL,1,0,0,0);
/*!40000 ALTER TABLE `tel` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `tel_types`
--

DROP TABLE IF EXISTS `tel_types`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `tel_types` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `name` tinytext NOT NULL,
  `mask` tinytext NOT NULL,
  `order` int(11) unsigned NOT NULL,
  `enabled` bit(1) NOT NULL DEFAULT b'1',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=4 DEFAULT CHARSET=utf8mb4;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `tel_types`
--

LOCK TABLES `tel_types` WRITE;
/*!40000 ALTER TABLE `tel_types` DISABLE KEYS */;
INSERT INTO `tel_types` VALUES (1,'мобильный (UA)','(+38) 999 999-99-99',2,0x00),(2,'городской','99-99-99',3,0x01),(3,'мобильный (RU)','+7 999 999-99-99',1,0x01);
/*!40000 ALTER TABLE `tel_types` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `user_params`
--

DROP TABLE IF EXISTS `user_params`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `user_params` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `name` tinytext NOT NULL,
  `default_value` text,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `user_params`
--

LOCK TABLES `user_params` WRITE;
/*!40000 ALTER TABLE `user_params` DISABLE KEYS */;
/*!40000 ALTER TABLE `user_params` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `users`
--

DROP TABLE IF EXISTS `users`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `users` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `sip_user_id` int(11) DEFAULT NULL,
  `client_user_id` int(11) DEFAULT NULL COMMENT 'id карточки клиента-сотрудника',
  `username` varchar(45) NOT NULL,
  `name` varchar(254) NOT NULL COMMENT 'Имя',
  `surname` varchar(254) DEFAULT NULL COMMENT 'Фамилия',
  `patronymic` varchar(254) DEFAULT NULL COMMENT 'Отчество',
  `phone` varchar(45) DEFAULT NULL,
  `phone2` varchar(50) DEFAULT NULL,
  `phone_mask` int(11) NOT NULL DEFAULT '1',
  `phone2_mask` int(11) NOT NULL DEFAULT '1',
  `address` varchar(254) DEFAULT NULL COMMENT 'Адрес',
  `passport_num` varchar(254) DEFAULT NULL COMMENT 'Паспортные данные',
  `passport_date` date DEFAULT NULL,
  `passport_organ` varchar(254) DEFAULT NULL,
  `state` int(11) DEFAULT '1' COMMENT 'Состояние пользователя\n0 архивный\n1 активный',
  `created` datetime DEFAULT NULL,
  `office` int(11) NOT NULL,
  `birthday` datetime DEFAULT NULL,
  `def_office` int(11) NOT NULL DEFAULT '0' COMMENT 'ОФИС В СПИСКЕ РЕМОНТОВ ПО УМОЛЧАНИЮ',
  `def_store` int(11) NOT NULL DEFAULT '0' COMMENT 'СКЛАД ПО УМОЛЧАНИЮ',
  `def_item_state` int(11) NOT NULL DEFAULT '0' COMMENT 'ТОВАРЫ ПО УМОЛЧАНИЮ (резерв, только наличие и тп)',
  `def_employee` int(11) NOT NULL DEFAULT '0' COMMENT 'СОТРУДНИК В СПИСКЕ РЕМОНТОВ ПО УМОЛЧАНИЮ',
  `def_status` int(11) DEFAULT NULL COMMENT 'СТАТУС В СПИСКЕ РЕМОНТОВ ПО УМОЛЧАНИЮ',
  `def_ws_filter` int(11) NOT NULL DEFAULT '0' COMMENT 'Без картриджей/Все/Только картриджи',
  `last_login` datetime DEFAULT NULL,
  `last_activity` datetime DEFAULT NULL,
  `email` varchar(254) DEFAULT NULL,
  `sex` int(1) DEFAULT NULL COMMENT '0 = не указан; 1 = М; 2 = Ж',
  `photo` mediumblob,
  `salary_rate` int(11) DEFAULT NULL,
  `pay_day` int(11) NOT NULL DEFAULT '0',
  `pay_day_off` int(11) NOT NULL DEFAULT '0',
  `pay_repair` int(11) NOT NULL DEFAULT '0' COMMENT 'Процент от выполненных работ',
  `pay_repair_quick` int(11) NOT NULL DEFAULT '0',
  `pay_sale` int(11) NOT NULL DEFAULT '0' COMMENT 'Процент от прибыли с продажи товаров',
  `pay_repair_q_sale` int(11) NOT NULL DEFAULT '0' COMMENT 'Процент с работ при быстрой продаже',
  `pay_cartridge_refill` int(11) NOT NULL DEFAULT '0',
  `pay_device_in` int(11) NOT NULL DEFAULT '0' COMMENT 'Сумма за принятый ремонт',
  `pay_device_out` int(11) NOT NULL DEFAULT '0' COMMENT 'Сумма за выданный ремонт',
  `pay_4_sale_in_repair` tinyint(1) NOT NULL DEFAULT '0' COMMENT 'Начисление за продажу деталей в ремонте',
  `row_color` char(9) NOT NULL DEFAULT '#D8008EA4',
  `ge_highlight_color` char(9) NOT NULL DEFAULT '#FFFFDD70' COMMENT 'Цвет подсветки в групповом редакторе товара',
  `color_label_ws` char(9) NOT NULL DEFAULT '#FF000000',
  `workspace_mode` int(11) NOT NULL DEFAULT '0',
  `preview_before_print` tinyint(1) NOT NULL DEFAULT '1',
  `new_rep_doc_copies` int(11) NOT NULL DEFAULT '2',
  `auto_refresh_workspace` tinyint(1) NOT NULL DEFAULT '1',
  `refresh_time` int(11) NOT NULL DEFAULT '10' COMMENT 'auto refresh time...',
  `xls_c1` int(11) NOT NULL DEFAULT '0',
  `xls_c2` int(11) NOT NULL DEFAULT '1',
  `xls_c3` int(11) NOT NULL DEFAULT '2',
  `xls_c4` int(11) NOT NULL DEFAULT '3',
  `xls_c5` int(11) NOT NULL DEFAULT '4',
  `xls_c6` int(11) NOT NULL DEFAULT '5',
  `xls_c7` int(11) NOT NULL DEFAULT '6',
  `xls_c8` int(11) NOT NULL DEFAULT '7',
  `xls_c9` int(11) NOT NULL DEFAULT '8',
  `xls_c10` int(11) NOT NULL DEFAULT '9',
  `xls_c11` int(11) NOT NULL DEFAULT '10',
  `xls_c12` int(11) NOT NULL DEFAULT '11',
  `xls_c13` int(11) NOT NULL DEFAULT '12',
  `xls_c14` int(11) NOT NULL DEFAULT '13',
  `xls_c15` int(11) NOT NULL DEFAULT '14',
  `display_out` tinyint(1) NOT NULL DEFAULT '1',
  `display_complete` tinyint(1) NOT NULL DEFAULT '1',
  `is_bot` tinyint(1) NOT NULL DEFAULT '0',
  `new_on_top` tinyint(1) NOT NULL DEFAULT '0',
  `issued_color` char(9) NOT NULL DEFAULT '#00000000',
  `fields_cfg` text,
  `save_state_on_close` tinyint(1) NOT NULL DEFAULT '1',
  `group_store_items` tinyint(1) NOT NULL DEFAULT '1',
  `track_activity` tinyint(1) NOT NULL DEFAULT '1',
  `card_on_call` tinyint(1) NOT NULL DEFAULT '1' COMMENT 'Открывать сводку по клиенту при входящем вызове',
  `inn` varchar(50) DEFAULT NULL,
  `inform_comment` tinyint(1) NOT NULL DEFAULT '1' COMMENT 'Информирование о новом комментарии к ремонту',
  `inform_status` tinyint(1) NOT NULL DEFAULT '1' COMMENT 'Информитрование о изменении статуса ремонта',
  `kkt` int(11) DEFAULT NULL,
  `pinpad` int(11) DEFAULT NULL,
  `advance_disable` tinyint(1) NOT NULL DEFAULT '0',
  `salary_disable` tinyint(1) NOT NULL DEFAULT '0',
  `notes` text,
  `signature` text COMMENT 'подпись в сообщениях',
  `kkm_pass` int(11) DEFAULT NULL,
  `prefer_regular` tinyint(1) NOT NULL DEFAULT '1',
  `fontsize` int(11) NOT NULL DEFAULT '13',
  `rowheight` int(11) NOT NULL DEFAULT '30',
  `animation` varchar(50) NOT NULL DEFAULT 'SlideHorizontal',
  PRIMARY KEY (`id`),
  UNIQUE KEY `username` (`username`),
  KEY `fk_users_offices1_idx` (`office`),
  KEY `fk_users_ps_auths1_idx` (`sip_user_id`),
  KEY `state` (`state`),
  KEY `track_activity` (`track_activity`),
  KEY `FK_users_kkt` (`kkt`),
  KEY `FK_users_pinpad` (`pinpad`),
  KEY `fk_users_clients` (`client_user_id`),
  CONSTRAINT `FK_users_kkt` FOREIGN KEY (`kkt`) REFERENCES `kkt` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT `FK_users_pinpad` FOREIGN KEY (`pinpad`) REFERENCES `pinpad` (`id`) ON DELETE SET NULL ON UPDATE NO ACTION,
  CONSTRAINT `fk_users_clients` FOREIGN KEY (`client_user_id`) REFERENCES `clients` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT `fk_users_offices1` FOREIGN KEY (`office`) REFERENCES `offices` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION
) ENGINE=InnoDB AUTO_INCREMENT=8 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `users`
--

LOCK TABLES `users` WRITE;
/*!40000 ALTER TABLE `users` DISABLE KEYS */;
INSERT INTO `users` VALUES (1,NULL,NULL,'admin','Администратор','','','','',1,1,NULL,NULL,NULL,NULL,1,'2025-01-01 09:00:00',1,NULL,1,1,0,0,99,2,'2025-11-18 01:11:54','2025-11-17 23:52:58','',NULL,NULL,NULL,0,0,50,50,0,0,0,0,0,0,'#D8008EA4','#FFFFDD70','#FF000000',0,1,0,1,10,0,1,2,0,10,99,3,8,5,6,7,4,9,0,99,1,1,0,1,'#00000000','[{\"Name\":\"ws_vis_id\",\"Visible\":true},{\"Name\":\"ws_vis_device\",\"Visible\":true},{\"Name\":\"ws_vis_sn\",\"Visible\":true},{\"Name\":\"ws_vis_fault\",\"Visible\":true},{\"Name\":\"ws_vis_master\",\"Visible\":true},{\"Name\":\"ws_vis_manager\",\"Visible\":false},{\"Name\":\"ws_vis_state\",\"Visible\":true},{\"Name\":\"ws_vis_summ\",\"Visible\":true},{\"Name\":\"ws_vis_cl\",\"Visible\":false},{\"Name\":\"ws_vis_cl_phone\",\"Visible\":false},{\"Name\":\"ws_vis_box\",\"Visible\":false},{\"Name\":\"ws_vis_date\",\"Visible\":true}]',1,1,0,0,NULL,1,1,NULL,NULL,0,0,NULL,NULL,NULL,1,13,30,'None'),(2,NULL,NULL,'test','Инженер','','','','',1,1,NULL,NULL,NULL,NULL,1,'2025-01-01 09:00:00',1,NULL,1,1,0,0,99,2,NULL,NULL,'',NULL,NULL,NULL,0,0,50,50,0,0,0,0,0,0,'#D8008EA4','#FFFFDD70','#FF000000',0,1,0,1,10,0,1,2,0,10,99,3,8,5,6,7,4,9,0,99,1,1,0,1,'#00000000','[{\"Name\":\"ws_vis_id\",\"Visible\":true},{\"Name\":\"ws_vis_device\",\"Visible\":true},{\"Name\":\"ws_vis_sn\",\"Visible\":true},{\"Name\":\"ws_vis_fault\",\"Visible\":true},{\"Name\":\"ws_vis_master\",\"Visible\":true},{\"Name\":\"ws_vis_manager\",\"Visible\":false},{\"Name\":\"ws_vis_state\",\"Visible\":true},{\"Name\":\"ws_vis_summ\",\"Visible\":true},{\"Name\":\"ws_vis_cl\",\"Visible\":false},{\"Name\":\"ws_vis_cl_phone\",\"Visible\":false},{\"Name\":\"ws_vis_box\",\"Visible\":false},{\"Name\":\"ws_vis_date\",\"Visible\":true}]',1,1,0,0,NULL,1,1,NULL,NULL,0,0,NULL,NULL,NULL,1,13,30,'None'),(3,NULL,NULL,'user3','Менеджер','','','','',1,1,NULL,NULL,NULL,NULL,1,'2025-01-01 09:00:00',1,NULL,1,1,0,0,99,2,NULL,NULL,'',NULL,NULL,NULL,0,0,50,50,0,0,0,0,0,0,'#D8008EA4','#FFFFDD70','#FF000000',0,1,0,1,10,0,1,2,0,10,99,3,8,5,6,7,4,9,0,99,1,1,0,1,'#00000000','[{\"Name\":\"ws_vis_id\",\"Visible\":true},{\"Name\":\"ws_vis_device\",\"Visible\":true},{\"Name\":\"ws_vis_sn\",\"Visible\":true},{\"Name\":\"ws_vis_fault\",\"Visible\":true},{\"Name\":\"ws_vis_master\",\"Visible\":true},{\"Name\":\"ws_vis_manager\",\"Visible\":false},{\"Name\":\"ws_vis_state\",\"Visible\":true},{\"Name\":\"ws_vis_summ\",\"Visible\":true},{\"Name\":\"ws_vis_cl\",\"Visible\":false},{\"Name\":\"ws_vis_cl_phone\",\"Visible\":false},{\"Name\":\"ws_vis_box\",\"Visible\":false},{\"Name\":\"ws_vis_date\",\"Visible\":true}]',1,1,0,0,NULL,1,1,NULL,NULL,0,0,NULL,NULL,NULL,1,13,30,'None'),(4,NULL,NULL,'user4','Директор','','','','',1,1,NULL,NULL,NULL,NULL,1,'2025-01-01 09:00:00',1,NULL,1,1,0,0,99,2,NULL,NULL,'',NULL,NULL,NULL,0,0,50,50,0,0,0,0,0,0,'#D8008EA4','#FFFFDD70','#FF000000',0,1,0,1,10,0,1,2,0,10,99,3,8,5,6,7,4,9,0,99,1,1,0,1,'#00000000','[{\"Name\":\"ws_vis_id\",\"Visible\":true},{\"Name\":\"ws_vis_device\",\"Visible\":true},{\"Name\":\"ws_vis_sn\",\"Visible\":true},{\"Name\":\"ws_vis_fault\",\"Visible\":true},{\"Name\":\"ws_vis_master\",\"Visible\":true},{\"Name\":\"ws_vis_manager\",\"Visible\":false},{\"Name\":\"ws_vis_state\",\"Visible\":true},{\"Name\":\"ws_vis_summ\",\"Visible\":true},{\"Name\":\"ws_vis_cl\",\"Visible\":false},{\"Name\":\"ws_vis_cl_phone\",\"Visible\":false},{\"Name\":\"ws_vis_box\",\"Visible\":false},{\"Name\":\"ws_vis_date\",\"Visible\":true}]',1,1,0,0,NULL,1,1,NULL,NULL,0,0,NULL,NULL,NULL,1,13,30,'None'),(5,NULL,NULL,'user5','Старший инженер','','','','',1,1,NULL,NULL,NULL,NULL,1,'2025-01-01 09:00:00',1,NULL,1,1,0,0,99,2,NULL,NULL,'',NULL,NULL,NULL,0,0,50,50,0,0,0,0,0,0,'#D8008EA4','#FFFFDD70','#FF000000',0,1,0,1,10,0,1,2,0,10,99,3,8,5,6,7,4,9,0,99,1,1,0,1,'#00000000','[{\"Name\":\"ws_vis_id\",\"Visible\":true},{\"Name\":\"ws_vis_device\",\"Visible\":true},{\"Name\":\"ws_vis_sn\",\"Visible\":true},{\"Name\":\"ws_vis_fault\",\"Visible\":true},{\"Name\":\"ws_vis_master\",\"Visible\":true},{\"Name\":\"ws_vis_manager\",\"Visible\":false},{\"Name\":\"ws_vis_state\",\"Visible\":true},{\"Name\":\"ws_vis_summ\",\"Visible\":true},{\"Name\":\"ws_vis_cl\",\"Visible\":false},{\"Name\":\"ws_vis_cl_phone\",\"Visible\":false},{\"Name\":\"ws_vis_box\",\"Visible\":false},{\"Name\":\"ws_vis_date\",\"Visible\":true}]',1,1,0,0,NULL,1,1,NULL,NULL,0,0,NULL,NULL,NULL,1,13,30,'None'),(6,NULL,NULL,'user6','старший менеджер ','','','','',1,1,NULL,NULL,NULL,NULL,1,'2025-01-01 09:00:00',1,NULL,1,1,0,0,99,2,NULL,NULL,'',NULL,NULL,NULL,0,0,50,50,0,0,0,0,0,0,'#D8008EA4','#FFFFDD70','#FF000000',0,1,0,1,10,0,1,2,0,10,99,3,8,5,6,7,4,9,0,99,1,1,0,1,'#00000000','[{\"Name\":\"ws_vis_id\",\"Visible\":true},{\"Name\":\"ws_vis_device\",\"Visible\":true},{\"Name\":\"ws_vis_sn\",\"Visible\":true},{\"Name\":\"ws_vis_fault\",\"Visible\":true},{\"Name\":\"ws_vis_master\",\"Visible\":true},{\"Name\":\"ws_vis_manager\",\"Visible\":false},{\"Name\":\"ws_vis_state\",\"Visible\":true},{\"Name\":\"ws_vis_summ\",\"Visible\":true},{\"Name\":\"ws_vis_cl\",\"Visible\":false},{\"Name\":\"ws_vis_cl_phone\",\"Visible\":false},{\"Name\":\"ws_vis_box\",\"Visible\":false},{\"Name\":\"ws_vis_date\",\"Visible\":true}]',1,1,0,0,NULL,1,1,NULL,NULL,0,0,NULL,NULL,NULL,1,13,30,'None'),(7,NULL,NULL,'user7','Инженер','','','','',1,1,NULL,NULL,NULL,NULL,1,'2025-01-01 09:00:00',1,NULL,1,1,0,0,99,2,NULL,NULL,'',NULL,NULL,NULL,0,0,50,50,0,0,0,0,0,0,'#D8008EA4','#FFFFDD70','#FF000000',0,1,0,1,10,0,1,2,0,10,99,3,8,5,6,7,4,9,0,99,1,1,0,1,'#00000000','[{\"Name\":\"ws_vis_id\",\"Visible\":true},{\"Name\":\"ws_vis_device\",\"Visible\":true},{\"Name\":\"ws_vis_sn\",\"Visible\":true},{\"Name\":\"ws_vis_fault\",\"Visible\":true},{\"Name\":\"ws_vis_master\",\"Visible\":true},{\"Name\":\"ws_vis_manager\",\"Visible\":false},{\"Name\":\"ws_vis_state\",\"Visible\":true},{\"Name\":\"ws_vis_summ\",\"Visible\":true},{\"Name\":\"ws_vis_cl\",\"Visible\":false},{\"Name\":\"ws_vis_cl_phone\",\"Visible\":false},{\"Name\":\"ws_vis_box\",\"Visible\":false},{\"Name\":\"ws_vis_date\",\"Visible\":true}]',1,1,0,0,NULL,1,1,NULL,NULL,0,0,NULL,NULL,NULL,1,13,30,'None');
/*!40000 ALTER TABLE `users` ENABLE KEYS */;
UNLOCK TABLES;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8mb4 */ ;
/*!50003 SET character_set_results = utf8mb4 */ ;
/*!50003 SET collation_connection  = utf8mb4_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,STRICT_ALL_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ALLOW_INVALID_DATES,ERROR_FOR_DIVISION_BY_ZERO,TRADITIONAL,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50017 DEFINER=`root`@`%`*/ /*!50003 TRIGGER `users_BEFORE_INSERT` BEFORE INSERT ON `users` FOR EACH ROW BEGIN
SET NEW.created = NOW();
END */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;

--
-- Table structure for table `users_activity`
--

DROP TABLE IF EXISTS `users_activity`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `users_activity` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `user_id` int(11) NOT NULL,
  `datetime_` datetime NOT NULL,
  `address` varchar(255) DEFAULT NULL,
  `notes` varchar(254) NOT NULL,
  `app_version` varchar(55) DEFAULT NULL,
  `machine_name` varchar(255) DEFAULT NULL,
  PRIMARY KEY (`id`),
  KEY `FK_users_activity_users` (`user_id`),
  CONSTRAINT `FK_users_activity_users` FOREIGN KEY (`user_id`) REFERENCES `users` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `users_activity`
--

LOCK TABLES `users_activity` WRITE;
/*!40000 ALTER TABLE `users_activity` DISABLE KEYS */;
INSERT INTO `users_activity` VALUES (1,1,'2025-11-18 01:11:54','localhost','Выполнен вход в систему','3.7.37.1184','SOLDERER-HOME');
/*!40000 ALTER TABLE `users_activity` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `users_params`
--

DROP TABLE IF EXISTS `users_params`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `users_params` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `user_id` int(11) NOT NULL,
  `param_id` int(11) NOT NULL,
  `value` text,
  PRIMARY KEY (`id`),
  UNIQUE KEY `unique_user_param` (`user_id`,`param_id`),
  KEY `FK_params` (`param_id`),
  CONSTRAINT `FK_params` FOREIGN KEY (`param_id`) REFERENCES `user_params` (`id`),
  CONSTRAINT `FK_users` FOREIGN KEY (`user_id`) REFERENCES `users` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `users_params`
--

LOCK TABLES `users_params` WRITE;
/*!40000 ALTER TABLE `users_params` DISABLE KEYS */;
/*!40000 ALTER TABLE `users_params` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `vat_invoice`
--

DROP TABLE IF EXISTS `vat_invoice`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `vat_invoice` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `num` varchar(10) NOT NULL,
  `created` datetime NOT NULL,
  `invoice` int(11) DEFAULT NULL,
  `user` int(11) NOT NULL,
  `seller` int(11) NOT NULL,
  `customer` int(11) NOT NULL,
  `office` int(11) NOT NULL,
  `tax` decimal(19,4) NOT NULL DEFAULT '0.0000',
  `summ` decimal(19,4) NOT NULL DEFAULT '0.0000',
  `total` decimal(19,4) NOT NULL DEFAULT '0.0000',
  `notes` varchar(254) DEFAULT NULL,
  `currency` varchar(50) NOT NULL,
  `currency_code` varchar(10) NOT NULL,
  `state_contract` varchar(254) DEFAULT NULL,
  `payment_order` varchar(10) DEFAULT NULL,
  `payment_order_date` date DEFAULT NULL,
  PRIMARY KEY (`id`),
  KEY `num` (`num`),
  KEY `created` (`created`),
  KEY `FK_vat_invoice_invoice` (`invoice`),
  KEY `FK_vat_invoice_users` (`user`),
  KEY `FK_vat_invoice_offices` (`office`),
  KEY `FK_vat_invoice_seller` (`seller`),
  KEY `FK_vat_invoice_customer` (`customer`),
  CONSTRAINT `FK_vat_invoice_customer` FOREIGN KEY (`customer`) REFERENCES `banks` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT `FK_vat_invoice_invoice` FOREIGN KEY (`invoice`) REFERENCES `invoice` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT `FK_vat_invoice_offices` FOREIGN KEY (`office`) REFERENCES `offices` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT `FK_vat_invoice_seller` FOREIGN KEY (`seller`) REFERENCES `banks` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT `FK_vat_invoice_users` FOREIGN KEY (`user`) REFERENCES `users` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `vat_invoice`
--

LOCK TABLES `vat_invoice` WRITE;
/*!40000 ALTER TABLE `vat_invoice` DISABLE KEYS */;
/*!40000 ALTER TABLE `vat_invoice` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `vendors`
--

DROP TABLE IF EXISTS `vendors`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `vendors` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(254) NOT NULL DEFAULT '0',
  `archive` tinyint(1) NOT NULL DEFAULT '0',
  `created_at` datetime NOT NULL DEFAULT '0000-00-00 00:00:00',
  `updated_at` datetime DEFAULT NULL,
  `parts_agent` int(11) NOT NULL,
  `works_agent` int(11) NOT NULL,
  PRIMARY KEY (`id`),
  KEY `FK_vendors_clients` (`parts_agent`),
  KEY `FK_vendors_clients_2` (`works_agent`),
  CONSTRAINT `FK_vendors_clients` FOREIGN KEY (`parts_agent`) REFERENCES `clients` (`id`) ON UPDATE CASCADE,
  CONSTRAINT `FK_vendors_clients_2` FOREIGN KEY (`works_agent`) REFERENCES `clients` (`id`) ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `vendors`
--

LOCK TABLES `vendors` WRITE;
/*!40000 ALTER TABLE `vendors` DISABLE KEYS */;
/*!40000 ALTER TABLE `vendors` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `visit_sources`
--

DROP TABLE IF EXISTS `visit_sources`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `visit_sources` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(254) NOT NULL,
  `position` int(11) DEFAULT NULL,
  `enabled` tinyint(1) DEFAULT '1',
  `fire_on` int(11) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=13 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `visit_sources`
--

LOCK TABLES `visit_sources` WRITE;
/*!40000 ALTER TABLE `visit_sources` DISABLE KEYS */;
INSERT INTO `visit_sources` VALUES (1,'Подсказали знакомые',1,1,NULL),(2,'Реклама на радио',2,1,NULL),(3,'Бигборд (рекламный щит)',3,1,NULL),(4,'Флаер (рекламная листовка)',4,1,NULL),(5,'Поисковый запрос в Internet',5,1,NULL),(6,'Направили из магазина по вопросам гарантии',6,1,NULL),(7,'Постоянный клиент',NULL,1,2),(8,'повторное обращение',NULL,1,3),(9,'Гарантийный ремонт',NULL,1,4),(10,'Прислали из другого сервисного центра',NULL,1,NULL),(11,'Инстаграм',NULL,1,NULL),(12,'Проживает рядом',7,1,NULL);
/*!40000 ALTER TABLE `visit_sources` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `voicemail`
--

DROP TABLE IF EXISTS `voicemail`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `voicemail` (
  `uniqueid` int(11) NOT NULL AUTO_INCREMENT,
  `context` varchar(80) NOT NULL,
  `mailbox` varchar(80) NOT NULL,
  `password` varchar(80) NOT NULL,
  `fullname` varchar(80) DEFAULT NULL,
  `alias` varchar(80) DEFAULT NULL,
  `email` varchar(80) DEFAULT NULL,
  `pager` varchar(80) DEFAULT NULL,
  `attach` enum('yes','no') DEFAULT NULL,
  `attachfmt` varchar(10) DEFAULT NULL,
  `serveremail` varchar(80) DEFAULT NULL,
  `language` varchar(20) DEFAULT NULL,
  `tz` varchar(30) DEFAULT NULL,
  `deletevoicemail` enum('yes','no') DEFAULT NULL,
  `saycid` enum('yes','no') DEFAULT NULL,
  `sendvoicemail` enum('yes','no') DEFAULT NULL,
  `review` enum('yes','no') DEFAULT NULL,
  `tempgreetwarn` enum('yes','no') DEFAULT NULL,
  `operator` enum('yes','no') DEFAULT NULL,
  `envelope` enum('yes','no') DEFAULT NULL,
  `sayduration` int(11) DEFAULT NULL,
  `forcename` enum('yes','no') DEFAULT NULL,
  `forcegreetings` enum('yes','no') DEFAULT NULL,
  `callback` varchar(80) DEFAULT NULL,
  `dialout` varchar(80) DEFAULT NULL,
  `exitcontext` varchar(80) DEFAULT NULL,
  `maxmsg` int(11) DEFAULT NULL,
  `volgain` decimal(5,2) DEFAULT NULL,
  `imapuser` varchar(80) DEFAULT NULL,
  `imappassword` varchar(80) DEFAULT NULL,
  `imapserver` varchar(80) DEFAULT NULL,
  `imapport` varchar(8) DEFAULT NULL,
  `imapflags` varchar(80) DEFAULT NULL,
  `stamp` datetime DEFAULT NULL,
  PRIMARY KEY (`uniqueid`),
  KEY `voicemail_mailbox` (`mailbox`),
  KEY `voicemail_context` (`context`),
  KEY `voicemail_mailbox_context` (`mailbox`,`context`),
  KEY `voicemail_imapuser` (`imapuser`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `voicemail`
--

LOCK TABLES `voicemail` WRITE;
/*!40000 ALTER TABLE `voicemail` DISABLE KEYS */;
/*!40000 ALTER TABLE `voicemail` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `voip_ext_devices`
--

DROP TABLE IF EXISTS `voip_ext_devices`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `voip_ext_devices` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(254) NOT NULL DEFAULT '',
  `phone` varchar(254) NOT NULL DEFAULT '',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `voip_ext_devices`
--

LOCK TABLES `voip_ext_devices` WRITE;
/*!40000 ALTER TABLE `voip_ext_devices` DISABLE KEYS */;
/*!40000 ALTER TABLE `voip_ext_devices` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `w_lists`
--

DROP TABLE IF EXISTS `w_lists`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `w_lists` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `num` varchar(10) NOT NULL,
  `created` datetime NOT NULL,
  `invoice` int(11) DEFAULT NULL,
  `user` int(11) NOT NULL,
  `seller` int(11) NOT NULL,
  `customer` int(11) NOT NULL,
  `office` int(11) NOT NULL,
  `tax` decimal(19,4) NOT NULL DEFAULT '0.0000',
  `summ` decimal(19,4) NOT NULL DEFAULT '0.0000',
  `total` decimal(19,4) NOT NULL DEFAULT '0.0000',
  `notes` varchar(254) DEFAULT NULL,
  `reason` varchar(254) DEFAULT NULL,
  PRIMARY KEY (`id`),
  KEY `FK_w_lists_invoice` (`invoice`),
  KEY `FK_w_lists_users` (`user`),
  KEY `FK_w_lists_offices` (`office`),
  KEY `num` (`num`),
  KEY `created` (`created`),
  KEY `FK_w_lists_seller` (`seller`),
  KEY `FK_w_lists_customer` (`customer`),
  CONSTRAINT `FK_w_lists_customer` FOREIGN KEY (`customer`) REFERENCES `banks` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT `FK_w_lists_invoice` FOREIGN KEY (`invoice`) REFERENCES `invoice` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT `FK_w_lists_offices` FOREIGN KEY (`office`) REFERENCES `offices` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT `FK_w_lists_seller` FOREIGN KEY (`seller`) REFERENCES `banks` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT `FK_w_lists_users` FOREIGN KEY (`user`) REFERENCES `users` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Акты на выполнение работ-услуг';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `w_lists`
--

LOCK TABLES `w_lists` WRITE;
/*!40000 ALTER TABLE `w_lists` DISABLE KEYS */;
/*!40000 ALTER TABLE `w_lists` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `works`
--

DROP TABLE IF EXISTS `works`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `works` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `user` int(11) NOT NULL,
  `repair` int(11) DEFAULT NULL,
  `document_id` int(11) DEFAULT NULL,
  `name` varchar(254) NOT NULL,
  `price` decimal(19,4) NOT NULL DEFAULT '0.0000',
  `count` int(10) unsigned NOT NULL DEFAULT '1',
  `warranty` int(11) NOT NULL,
  `price_id` int(11) DEFAULT NULL,
  `is_pay` tinyint(1) NOT NULL DEFAULT '0',
  `added` datetime DEFAULT NULL,
  `type` int(11) NOT NULL DEFAULT '0',
  `pay_repair` int(11) DEFAULT NULL COMMENT 'процент с работ',
  `pay_repair_quick` int(11) DEFAULT NULL COMMENT 'процент с работ в быстром ремонте',
  `salary_summ` decimal(19,4) DEFAULT NULL COMMENT 'сумма выплаты сотруднику',
  PRIMARY KEY (`id`),
  KEY `fk_works_workshop1_idx` (`repair`),
  KEY `fk_works_users1_idx` (`user`),
  KEY `fk_works_workshop_price1_idx` (`price_id`),
  KEY `fk_works_docs1_idx` (`document_id`),
  KEY `type` (`type`),
  CONSTRAINT `fk_works_docs1` FOREIGN KEY (`document_id`) REFERENCES `docs` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT `fk_works_users1` FOREIGN KEY (`user`) REFERENCES `users` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT `fk_works_workshop1` FOREIGN KEY (`repair`) REFERENCES `workshop` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT `fk_works_workshop_price1` FOREIGN KEY (`price_id`) REFERENCES `workshop_price` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION
) ENGINE=InnoDB AUTO_INCREMENT=14 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `works`
--

LOCK TABLES `works` WRITE;
/*!40000 ALTER TABLE `works` DISABLE KEYS */;
INSERT INTO `works` VALUES (1,2,1,NULL,'Разборка/сборка (полная)',2014.0000,1,0,NULL,0,'2025-11-20 03:49:04',0,50,50,NULL);
/*!40000 ALTER TABLE `works` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `workshop`
--

DROP TABLE IF EXISTS `workshop`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `workshop` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `Hidden` tinyint(1) NOT NULL DEFAULT '0',
  `Title` varchar(1000) NOT NULL DEFAULT '',
  `client` int(11) NOT NULL,
  `type` int(11) NOT NULL,
  `maker` int(11) NOT NULL,
  `model` int(11) DEFAULT NULL,
  `serial_number` varchar(254) DEFAULT NULL,
  `company` int(11) NOT NULL DEFAULT '0',
  `office` int(11) NOT NULL,
  `start_office` int(11) NOT NULL DEFAULT '0',
  `manager` int(11) NOT NULL,
  `current_manager` int(11) NOT NULL DEFAULT '0',
  `master` int(11) DEFAULT '0',
  `diagnostic_result` text,
  `in_date` datetime NOT NULL,
  `out_date` datetime DEFAULT NULL,
  `state` int(11) NOT NULL DEFAULT '0' COMMENT 'Статус ремонта\n0 принято в ремонт',
  `new_state` int(11) NOT NULL DEFAULT '0',
  `user_lock` int(11) DEFAULT NULL,
  `lock_datetime` datetime DEFAULT NULL,
  `express_repair` tinyint(1) DEFAULT '0',
  `quick_repair` tinyint(1) NOT NULL DEFAULT '0' COMMENT 'Быстрый ремонт',
  `is_warranty` tinyint(1) NOT NULL DEFAULT '0',
  `is_repeat` tinyint(1) NOT NULL DEFAULT '0',
  `payment_system` int(11) NOT NULL DEFAULT '0',
  `is_card_payment` tinyint(1) NOT NULL DEFAULT '0',
  `can_format` tinyint(1) NOT NULL DEFAULT '0' COMMENT 'Можно удалять данные',
  `print_check` tinyint(1) NOT NULL DEFAULT '0' COMMENT 'Напоминание менеджеру при выдаче устройства из ремонта о печати/выдаче чека',
  `box` int(11) DEFAULT NULL,
  `warranty_label` varchar(254) DEFAULT NULL,
  `ext_notes` text COMMENT 'Заметки/примечание для квитанции',
  `is_prepaid` tinyint(1) NOT NULL DEFAULT '0',
  `prepaid_type` int(11) NOT NULL DEFAULT '0',
  `prepaid_summ` decimal(19,4) NOT NULL DEFAULT '0.0000',
  `prepaid_order` int(11) DEFAULT '0' COMMENT 'ID записи о предоплате',
  `is_pre_agreed` tinyint(1) NOT NULL DEFAULT '0',
  `is_debt` tinyint(1) NOT NULL DEFAULT '0' COMMENT 'выдан в долг',
  `pre_agreed_amount` decimal(19,4) DEFAULT NULL,
  `repair_cost` decimal(19,4) NOT NULL DEFAULT '0.0000' COMMENT 'итоговая стоимость ремонта',
  `real_repair_cost` decimal(19,4) NOT NULL DEFAULT '0.0000',
  `parts_cost` decimal(19,4) NOT NULL DEFAULT '0.0000' COMMENT 'стоимость установленных запчастей',
  `fault` varchar(254) NOT NULL COMMENT 'Неисправность устройства',
  `complect` varchar(254) DEFAULT NULL COMMENT 'Что было в комплекте с устройством',
  `look` varchar(254) DEFAULT NULL COMMENT 'Внешний вид устройства',
  `thirs_party_sc` tinyint(1) NOT NULL DEFAULT '0' COMMENT 'До нас было в другом СЦ',
  `last_save` datetime DEFAULT NULL,
  `last_status_changed` datetime DEFAULT NULL,
  `warranty_days` int(11) NOT NULL DEFAULT '0',
  `barcode` varchar(254) NOT NULL DEFAULT '0',
  `reject_reason` text NOT NULL,
  `informed_status` int(11) NOT NULL DEFAULT '0',
  `image_ids` text,
  `color` char(9) DEFAULT NULL COMMENT 'цвет строки в списке ремонтов',
  `order_moving` text COMMENT 'ID офисов',
  `early` int(11) DEFAULT NULL COMMENT 'Предыдущий/гарантийный ремонт',
  `ext_early` varchar(50) DEFAULT NULL COMMENT 'Предыдущий/гарантийный ремонт из другой базы',
  `issued_msg` varchar(254) DEFAULT NULL,
  `sms_inform` tinyint(1) NOT NULL DEFAULT '1' COMMENT 'Разрешить отправку SMS',
  `invoice` int(11) DEFAULT NULL,
  `cartridge` int(11) DEFAULT NULL,
  `vendor_id` int(11) DEFAULT NULL,
  `termsControl` tinyint(1) NOT NULL DEFAULT '1',
  PRIMARY KEY (`id`),
  KEY `fk_workshop_clients1_idx` (`client`),
  KEY `fk_workshop_device_models1_idx` (`model`),
  KEY `fk_workshop_users2_idx` (`manager`),
  KEY `fk_workshop_offices1_idx` (`office`),
  KEY `fk_workshop_device_makers2_idx` (`maker`),
  KEY `fk_workshop_devices1_idx` (`type`),
  KEY `fk_workshop_boxes1_idx` (`box`),
  KEY `fk_workshop_offices2_idx` (`start_office`),
  KEY `fk_workshop_companies1_idx` (`company`),
  KEY `fk_workshop_users1_idx` (`master`),
  KEY `fk_workshop_users3_idx` (`current_manager`),
  KEY `fk_workshop_lockusers_idx` (`user_lock`),
  KEY `state` (`state`),
  KEY `out_date` (`out_date`),
  KEY `FK_workshop_payment_systems` (`payment_system`),
  KEY `FK_workshop_workshop` (`early`),
  KEY `in_date` (`in_date`),
  KEY `FK_workshop_invoice` (`invoice`),
  KEY `FK_workshop_c_workshop` (`cartridge`),
  KEY `FK_workshop_vendors` (`vendor_id`),
  CONSTRAINT `FK_workshop_c_workshop` FOREIGN KEY (`cartridge`) REFERENCES `c_workshop` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT `FK_workshop_invoice` FOREIGN KEY (`invoice`) REFERENCES `invoice` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT `FK_workshop_payment_systems` FOREIGN KEY (`payment_system`) REFERENCES `payment_systems` (`system_id`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT `FK_workshop_vendors` FOREIGN KEY (`vendor_id`) REFERENCES `vendors` (`id`) ON DELETE SET NULL ON UPDATE CASCADE,
  CONSTRAINT `FK_workshop_workshop` FOREIGN KEY (`early`) REFERENCES `workshop` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT `fk_workshop_boxes1` FOREIGN KEY (`box`) REFERENCES `boxes` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT `fk_workshop_clients1` FOREIGN KEY (`client`) REFERENCES `clients` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT `fk_workshop_companies1` FOREIGN KEY (`company`) REFERENCES `companies` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT `fk_workshop_device_makers2` FOREIGN KEY (`maker`) REFERENCES `device_makers` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT `fk_workshop_device_models1` FOREIGN KEY (`model`) REFERENCES `device_models` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT `fk_workshop_devices1` FOREIGN KEY (`type`) REFERENCES `devices` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT `fk_workshop_lockusers` FOREIGN KEY (`user_lock`) REFERENCES `users` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT `fk_workshop_offices1` FOREIGN KEY (`office`) REFERENCES `offices` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT `fk_workshop_offices2` FOREIGN KEY (`start_office`) REFERENCES `offices` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT `fk_workshop_users1` FOREIGN KEY (`master`) REFERENCES `users` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT `fk_workshop_users2` FOREIGN KEY (`manager`) REFERENCES `users` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT `fk_workshop_users3` FOREIGN KEY (`current_manager`) REFERENCES `users` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION
) ENGINE=InnoDB AUTO_INCREMENT=16 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `workshop`
--

LOCK TABLES `workshop` WRITE;
/*!40000 ALTER TABLE `workshop` DISABLE KEYS */;
INSERT INTO `workshop` VALUES (1,0,'ноутбук',864,1,28,1,'0123456789',1,1,1,1,1,1,NULL,'2025-01-01 09:00:00',NULL,0,0,NULL,NULL,0,0,0,0,0,0,0,0,NULL,NULL,NULL,0,0,0.0000,0,0,0,NULL,0.0000,0.0000,0.0000,'',NULL,NULL,0,NULL,NULL,0,'0','',0,NULL,NULL,NULL,NULL,NULL,NULL,1,NULL,NULL,NULL,1),(2,0,'монитор',864,2,3,2,'S65D4FG66S',1,1,1,1,1,1,NULL,'2025-01-01 09:00:00',NULL,0,0,NULL,NULL,0,0,0,0,0,0,0,0,NULL,NULL,NULL,0,0,0.0000,0,0,0,NULL,0.0000,0.0000,0.0000,'',NULL,NULL,0,NULL,NULL,0,'0','',0,NULL,NULL,NULL,NULL,NULL,NULL,1,NULL,NULL,NULL,1),(3,0,'планшет',864,3,13,3,'39TY-09WRY',1,1,1,1,1,1,NULL,'2025-01-01 09:00:00',NULL,0,0,NULL,NULL,0,0,0,0,0,0,0,0,NULL,NULL,NULL,0,0,0.0000,0,0,0,NULL,0.0000,0.0000,0.0000,'',NULL,NULL,0,NULL,NULL,0,'0','',0,NULL,NULL,NULL,NULL,NULL,NULL,1,NULL,NULL,NULL,1),(4,0,'картридж',864,5,42,5,'1234',1,1,1,1,1,1,NULL,'2025-01-01 09:00:00',NULL,0,0,NULL,NULL,0,0,0,0,0,0,0,0,NULL,NULL,NULL,0,0,0.0000,0,0,0,NULL,0.0000,0.0000,0.0000,'',NULL,NULL,0,NULL,NULL,0,'0','',0,NULL,NULL,NULL,NULL,NULL,NULL,1,NULL,1,NULL,1),(5,0,'Гравицапа ГЦ-1000М',1952,6,1,6,'ГЦ082840У',0,0,0,0,0,0,NULL,'0000-00-00 00:00:00',NULL,0,0,NULL,NULL,0,0,0,0,0,0,0,0,NULL,NULL,'комментарий для квитанции',0,0,0.0000,0,0,0,NULL,0.0000,0.0000,0.0000,'не тянет','устройство','хорошее состояние',0,NULL,NULL,0,'0','',0,NULL,NULL,NULL,NULL,NULL,NULL,1,NULL,NULL,NULL,1);
/*!40000 ALTER TABLE `workshop` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `workshop_cats`
--

DROP TABLE IF EXISTS `workshop_cats`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `workshop_cats` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(254) NOT NULL,
  `parent` int(11) DEFAULT NULL,
  `position` int(11) DEFAULT NULL,
  `archive` tinyint(1) NOT NULL DEFAULT '0',
  `ico` varchar(50) DEFAULT 'Open_16x16.png',
  `vendor_id` int(11) DEFAULT NULL,
  PRIMARY KEY (`id`),
  KEY `fk_workshop_cats_workshop_cats1_idx` (`parent`),
  KEY `FK_workshop_cats_vendors` (`vendor_id`),
  CONSTRAINT `FK_workshop_cats_vendors` FOREIGN KEY (`vendor_id`) REFERENCES `vendors` (`id`) ON DELETE SET NULL ON UPDATE CASCADE,
  CONSTRAINT `fk_workshop_cats_workshop_cats1` FOREIGN KEY (`parent`) REFERENCES `workshop_cats` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `workshop_cats`
--

LOCK TABLES `workshop_cats` WRITE;
/*!40000 ALTER TABLE `workshop_cats` DISABLE KEYS */;
INSERT INTO `workshop_cats` VALUES (1,'',NULL,NULL,0,'Open_16x16.png',NULL);
/*!40000 ALTER TABLE `workshop_cats` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `workshop_issued`
--

DROP TABLE IF EXISTS `workshop_issued`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `workshop_issued` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `repair_id` int(11) NOT NULL,
  `employee_id` int(11) NOT NULL,
  `created_at` datetime NOT NULL,
  PRIMARY KEY (`id`),
  KEY `FK_workshop_issued_workshop` (`repair_id`),
  KEY `FK_workshop_issued_users` (`employee_id`),
  CONSTRAINT `FK_workshop_issued_users` FOREIGN KEY (`employee_id`) REFERENCES `users` (`id`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `FK_workshop_issued_workshop` FOREIGN KEY (`repair_id`) REFERENCES `workshop` (`id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `workshop_issued`
--

LOCK TABLES `workshop_issued` WRITE;
/*!40000 ALTER TABLE `workshop_issued` DISABLE KEYS */;
/*!40000 ALTER TABLE `workshop_issued` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `workshop_parts`
--

DROP TABLE IF EXISTS `workshop_parts`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `workshop_parts` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `part_id` int(11) NOT NULL,
  `count` int(11) DEFAULT '1',
  `master` int(11) NOT NULL,
  `state` int(11) DEFAULT '0' COMMENT 'Состояние',
  `repair_id` int(11) NOT NULL,
  PRIMARY KEY (`id`),
  KEY `fk_device_parts_store_items1_idx` (`part_id`),
  KEY `fk_device_parts_users1_idx` (`master`),
  KEY `fk_device_parts_workshop1_idx` (`repair_id`),
  CONSTRAINT `fk_device_parts_store_items1` FOREIGN KEY (`part_id`) REFERENCES `store_items` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT `fk_device_parts_users1` FOREIGN KEY (`master`) REFERENCES `users` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT `fk_device_parts_workshop1` FOREIGN KEY (`repair_id`) REFERENCES `workshop` (`id`) ON DELETE CASCADE ON UPDATE NO ACTION
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `workshop_parts`
--

LOCK TABLES `workshop_parts` WRITE;
/*!40000 ALTER TABLE `workshop_parts` DISABLE KEYS */;
/*!40000 ALTER TABLE `workshop_parts` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `workshop_price`
--

DROP TABLE IF EXISTS `workshop_price`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `workshop_price` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(254) NOT NULL,
  `category` int(11) NOT NULL,
  `warranty` int(11) DEFAULT NULL COMMENT 'Гарантия в днях',
  `price1` decimal(19,4) NOT NULL DEFAULT '0.0000',
  `price2` decimal(19,4) NOT NULL DEFAULT '0.0000',
  `master_part` int(11) NOT NULL DEFAULT '0',
  `enable` tinyint(1) NOT NULL DEFAULT '1',
  `notes` varchar(254) DEFAULT NULL,
  `vendor_id` int(11) DEFAULT NULL,
  PRIMARY KEY (`id`),
  KEY `fk_workshop_price_workshop_cats1_idx` (`category`),
  KEY `FK_workshop_price_vendors` (`vendor_id`),
  CONSTRAINT `FK_workshop_price_vendors` FOREIGN KEY (`vendor_id`) REFERENCES `vendors` (`id`) ON DELETE SET NULL ON UPDATE CASCADE,
  CONSTRAINT `fk_workshop_price_workshop_cats1` FOREIGN KEY (`category`) REFERENCES `workshop_cats` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `workshop_price`
--

LOCK TABLES `workshop_price` WRITE;
/*!40000 ALTER TABLE `workshop_price` DISABLE KEYS */;
INSERT INTO `workshop_price` VALUES (1,'Работа из прайс-листа',1,NULL,1000.0000,950.0000,60,1,NULL,NULL);
/*!40000 ALTER TABLE `workshop_price` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Dumping routines for database 'snap_app_test'
--
/*!50003 DROP FUNCTION IF EXISTS `DiffDays` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`%` FUNCTION `DiffDays`(
	`datetime1` DATETIME,
	`datetime2` DATETIME
) RETURNS int(11)
BEGIN
RETURN datediff(datetime1, datetime2);
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `add_User` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8mb4 */ ;
/*!50003 SET character_set_results = utf8mb4 */ ;
/*!50003 SET collation_connection  = utf8mb4_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'NO_AUTO_VALUE_ON_ZERO' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`%` PROCEDURE `add_User`(IN `p_Name` VARCHAR(45), IN `p_Passw` VARCHAR(200), IN `p_DBName` VARCHAR(45))
BEGIN
DECLARE `_HOST` CHAR(14) DEFAULT '@\'%\'';
    SET `p_Name` := CONCAT('\'', REPLACE(TRIM(`p_Name`), CHAR(39), CONCAT(CHAR(92), CHAR(39))), '\''),
    `p_Passw` := CONCAT('\'', REPLACE(`p_Passw`, CHAR(39), CONCAT(CHAR(92), CHAR(39))), '\'');
    SET @`sql` := CONCAT('CREATE USER ', `p_Name`, `_HOST`, ' IDENTIFIED BY ', `p_Passw`);
    PREPARE `stmt` FROM @`sql`;
    EXECUTE `stmt`;
    SET @`sql` := CONCAT('GRANT ALL PRIVILEGES ON ', `p_DBName`,'.* TO ', `p_Name`, `_HOST`);
    PREPARE `stmt` FROM @`sql`;
    EXECUTE `stmt`;
    DEALLOCATE PREPARE `stmt`;
    FLUSH PRIVILEGES;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `change_Password` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8mb4 */ ;
/*!50003 SET character_set_results = utf8mb4 */ ;
/*!50003 SET collation_connection  = utf8mb4_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'NO_AUTO_VALUE_ON_ZERO' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`%` PROCEDURE `change_Password`()
BEGIN
 	DECLARE encrypted_password TINYTEXT DEFAULT PASSWORD(p_Passw);
	SET @_sql = CONCAT_WS('\'','SET PASSWORD FOR ',p_Name, ' = ',encrypted_password,'');

  PREPARE stmt FROM @_sql;
  EXECUTE stmt;
  DEALLOCATE PREPARE stmt;
  SET @_sql = NULL;
	FLUSH PRIVILEGES;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `delete_User` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8mb4 */ ;
/*!50003 SET character_set_results = utf8mb4 */ ;
/*!50003 SET collation_connection  = utf8mb4_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'NO_AUTO_VALUE_ON_ZERO' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`%` PROCEDURE `delete_User`(IN `p_Name` VARCHAR(45))
BEGIN
    SET @`sql` := CONCAT('DROP USER ', `p_Name`, '@\'%\'');
    PREPARE `stmt` FROM @`sql`;
    EXECUTE `stmt`;
    DEALLOCATE PREPARE `stmt`;
    FLUSH PRIVILEGES;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `getCatChildrens` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = '' */ ;
DELIMITER ;;
CREATE DEFINER=`admin`@`%` PROCEDURE `getCatChildrens`( categoryId INT(11) )
BEGIN
  DECLARE found INT(11) DEFAULT 1;
  DROP TABLE IF EXISTS cat_tree;
  CREATE TABLE cat_tree (cat_id int(11) PRIMARY KEY) ENGINE=HEAP;
  INSERT INTO cat_tree VALUES(categoryId);
  SET found = ROW_COUNT();
  WHILE found > 0 DO
    INSERT IGNORE INTO cat_tree
      SELECT c_child.id FROM cat_tree c JOIN store_cats c_child 
      WHERE c.cat_id = c_child.parent;
    SET found = ROW_COUNT();
  END WHILE;
  SELECT cat_id FROM cat_tree;
  DROP TABLE cat_tree;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2025-11-20  9:35:34

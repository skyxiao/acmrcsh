-- --------------------------------------------------------
-- 主机:                           127.0.0.1
-- 服务器版本:                        5.5.5-10.0.9-MariaDB - mariadb.org binary distribution
-- 服务器操作系统:                      Win32
-- HeidiSQL 版本:                  8.0.0.4396
-- --------------------------------------------------------

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

-- 导出 dry_etch 的数据库结构
CREATE DATABASE IF NOT EXISTS `dry_etch` /*!40100 DEFAULT CHARACTER SET utf8 */;
USE `dry_etch`;


-- 导出  过程 dry_etch.add_log 结构
DELIMITER //
CREATE DEFINER=`root`@`localhost` PROCEDURE `add_log`(IN `p_time` DATETIME, IN `p_id` INT, IN `p_level` ENUM('debug','info','warning','error','critical','emergency'), IN `p_info` TEXT)
    COMMENT 'record event log'
BEGIN
	insert into event_log values(p_time, p_id, p_level, p_info);
END//
DELIMITER ;


-- 导出  过程 dry_etch.create_wafer 结构
DELIMITER //
CREATE DEFINER=`root`@`localhost` PROCEDURE `create_wafer`(IN `p_id` CHAR(20), IN `p_type` ENUM('product','dummy'), IN `p_size` ENUM('100mm','150mm','200mm','300mm'), IN `p_time` DATETIME)
BEGIN
	insert into wafer_info values(p_id, p_type, p_size, 'unprocessed', p_time, null);
END//
DELIMITER ;


-- 导出  表 dry_etch.event_log 结构
CREATE TABLE IF NOT EXISTS `event_log` (
  `time` datetime NOT NULL,
  `id` int(10) unsigned NOT NULL,
  `level` enum('info','warning','error','fatal') NOT NULL,
  `info` text NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- 数据导出被取消选择。


-- 导出  过程 dry_etch.free 结构
DELIMITER //
CREATE DEFINER=`root`@`localhost` PROCEDURE `free`()
    COMMENT 'delete data of 1 month ago'
BEGIN
	declare expire_time datetime;
	set expire_time = date_sub(curdate(), interval 32 day);
	
	delete from event_log where time < expire_time;
	delete from process_data where time < expire_time;
	delete from wafer_movement where enter_time < expire_time;
	delete from wafer_process where start_time < expire_time;
	delete from wafer_info where remove_time < expire_time;
END//
DELIMITER ;


-- 导出  表 dry_etch.process_data 结构
CREATE TABLE IF NOT EXISTS `process_data` (
  `time` datetime NOT NULL,
  `type` enum('N2','EtOH','HF','Pressure','Chuck','Lid','Body') NOT NULL,
  `data` float NOT NULL DEFAULT '0',
  KEY `type` (`type`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- 数据导出被取消选择。


-- 导出  过程 dry_etch.process_end 结构
DELIMITER //
CREATE DEFINER=`root`@`localhost` PROCEDURE `process_end`(IN `p_id` CHAR(20), IN `p_state` ENUM('unprocessed','processed','semi-processed','broken'), IN `p_end_time` DATETIME)
BEGIN
	update wafer_process set end_time=p_end_time where id=p_id and end_time is null;
	update wafer_info set state=p_state where id=p_id;
END//
DELIMITER ;


-- 导出  过程 dry_etch.process_start 结构
DELIMITER //
CREATE DEFINER=`root`@`localhost` PROCEDURE `process_start`(IN `p_id` CHAR(20), IN `p_unit` TINYINT, IN `p_slot` TINYINT, IN `p_start_time` DATETIME, IN `p_recipe` CHAR(50))
BEGIN
	insert into wafer_process values(p_id, p_unit, p_slot, p_start_time, null, p_recipe);
END//
DELIMITER ;


-- 导出  过程 dry_etch.query_loaded_wafer 结构
DELIMITER //
CREATE DEFINER=`root`@`localhost` PROCEDURE `query_loaded_wafer`()
BEGIN
	select wafer_info.id, wafer_movement.unit, wafer_movement.slot, wafer_info.size, wafer_info.`type`, wafer_info.state 
	from wafer_info, wafer_movement where wafer_info.remove_time is null and wafer_info.id=wafer_movement.id and wafer_movement.exit_time is null
	order by wafer_info.create_time;
END//
DELIMITER ;


-- 导出  过程 dry_etch.record_data 结构
DELIMITER //
CREATE DEFINER=`root`@`localhost` PROCEDURE `record_data`(IN `p_time` DATETIME, IN `p_type` ENUM('N2','EtOH','HF','Pressure','Chuck','Lid','Body'), IN `p_value` FLOAT)
BEGIN
	insert into process_data values(p_time, p_type, p_value);
END//
DELIMITER ;


-- 导出  过程 dry_etch.remove_wafer 结构
DELIMITER //
CREATE DEFINER=`root`@`localhost` PROCEDURE `remove_wafer`(IN `p_id` CHAR(20), IN `p_time` DATETIME)
BEGIN
	update wafer_info set remove_time=p_time where id=p_id and remove_time is null;
END//
DELIMITER ;


-- 导出  表 dry_etch.user 结构
CREATE TABLE IF NOT EXISTS `user` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `name` char(50) NOT NULL DEFAULT '0',
  `password` char(50) NOT NULL DEFAULT '0',
  `overview` enum('invisible','readonly','operable') NOT NULL DEFAULT 'operable',
  `config` enum('invisible','readonly','operable') NOT NULL DEFAULT 'invisible',
  `data` enum('invisible','readonly','operable') NOT NULL DEFAULT 'invisible',
  `gasbox` enum('invisible','readonly','operable') NOT NULL DEFAULT 'operable',
  `recipe` enum('invisible','readonly','operable') NOT NULL DEFAULT 'invisible',
  `history` enum('invisible','readonly','operable') NOT NULL DEFAULT 'operable',
  `log` enum('invisible','readonly','operable') NOT NULL DEFAULT 'operable',
  `leakcheck` enum('invisible','readonly','operable') NOT NULL DEFAULT 'invisible',
  `monitor` enum('invisible','readonly','operable') NOT NULL DEFAULT 'invisible',
  `user` enum('invisible','readonly','operable') NOT NULL DEFAULT 'invisible',
  PRIMARY KEY (`id`),
  UNIQUE KEY `name` (`name`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- 数据导出被取消选择。


-- 导出  过程 dry_etch.wafer_enter 结构
DELIMITER //
CREATE DEFINER=`root`@`localhost` PROCEDURE `wafer_enter`(IN `p_id` CHAR(20), IN `p_unit` TINYINT, IN `p_slot` TINYINT, IN `p_enter_time` DATETIME, IN `p_enter_state` ENUM('unprocessed','processed','semi-processed','broken'))
BEGIN
	insert into wafer_movement values(p_id, p_unit, p_slot, p_enter_time, p_enter_state, null, p_enter_state);
END//
DELIMITER ;


-- 导出  过程 dry_etch.wafer_exit 结构
DELIMITER //
CREATE DEFINER=`root`@`localhost` PROCEDURE `wafer_exit`(IN `p_id` CHAR(20), IN `p_exit_time` DATETIME, IN `p_exit_state` ENUM('unprocessed','processed','semi-processed','broken'))
BEGIN
	update wafer_movement set exit_time=p_exit_time, exit_state=p_exit_state where id=p_id and exit_time is null;
END//
DELIMITER ;


-- 导出  表 dry_etch.wafer_info 结构
CREATE TABLE IF NOT EXISTS `wafer_info` (
  `id` char(20) NOT NULL,
  `type` enum('product','dummy') NOT NULL DEFAULT 'product',
  `size` enum('100mm','150mm','200mm','300mm') NOT NULL,
  `state` enum('unprocessed','processed','semi-processed','broken') NOT NULL DEFAULT 'unprocessed',
  `create_time` datetime NOT NULL,
  `remove_time` datetime DEFAULT NULL,
  PRIMARY KEY (`create_time`),
  UNIQUE KEY `id` (`id`),
  KEY `remove_time` (`remove_time`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- 数据导出被取消选择。


-- 导出  表 dry_etch.wafer_movement 结构
CREATE TABLE IF NOT EXISTS `wafer_movement` (
  `id` char(20) NOT NULL,
  `unit` tinyint(4) NOT NULL,
  `slot` tinyint(4) NOT NULL,
  `enter_time` datetime NOT NULL,
  `enter_state` enum('unprocessed','processed','semi-processed','broken') NOT NULL,
  `exit_time` datetime DEFAULT NULL,
  `exit_state` enum('unprocessed','processed','semi-processed','broken') DEFAULT NULL,
  KEY `FK_wafer_movement_wafer_info` (`id`),
  CONSTRAINT `FK_wafer_movement_wafer_info` FOREIGN KEY (`id`) REFERENCES `wafer_info` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- 数据导出被取消选择。


-- 导出  表 dry_etch.wafer_process 结构
CREATE TABLE IF NOT EXISTS `wafer_process` (
  `id` char(20) NOT NULL,
  `unit` tinyint(4) NOT NULL,
  `slot` tinyint(4) NOT NULL,
  `start_time` datetime NOT NULL,
  `end_time` datetime DEFAULT NULL,
  `recipe` char(50) NOT NULL,
  KEY `FK_wafer_process_wafer_info` (`id`),
  CONSTRAINT `FK_wafer_process_wafer_info` FOREIGN KEY (`id`) REFERENCES `wafer_info` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- 数据导出被取消选择。
/*!40101 SET SQL_MODE=IFNULL(@OLD_SQL_MODE, '') */;
/*!40014 SET FOREIGN_KEY_CHECKS=IF(@OLD_FOREIGN_KEY_CHECKS IS NULL, 1, @OLD_FOREIGN_KEY_CHECKS) */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;

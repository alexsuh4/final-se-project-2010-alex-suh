create database sampleprojectdb;

use sampleprojectdb;

delimiter ;
CREATE TABLE `datatypes` (
  `pkid` int(11) NOT NULL AUTO_INCREMENT,
  `dataTypeName` varchar(1000) NOT NULL COMMENT 'name of datatype',
  `metadata` varchar(1000) DEFAULT NULL COMMENT 'optional additional data',
  PRIMARY KEY (`pkid`),
  UNIQUE KEY `pkid_UNIQUE` (`pkid`)
) ENGINE=InnoDB AUTO_INCREMENT=30 DEFAULT CHARSET=utf8;

delimiter ;
CREATE TABLE `data` (
  `pkid` int(11) NOT NULL AUTO_INCREMENT,
  `dataType` int(11) NOT NULL DEFAULT '1' COMMENT 'hold key of datatype in datatypes table.',
  `data` varchar(1000) DEFAULT NULL COMMENT 'holds data\n',
  `metadata` varchar(1000) DEFAULT NULL COMMENT 'hold optional metadata of each row\n',
  PRIMARY KEY (`pkid`),
  UNIQUE KEY `pkid_UNIQUE` (`pkid`),
  KEY `dataTypeConnection` (`dataType`),
  CONSTRAINT `dataTypeConnection` FOREIGN KEY (`dataType`) REFERENCES `datatypes` (`pkid`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;


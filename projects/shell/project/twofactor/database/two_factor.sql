###########################################################################
#                                                                         #
#                         双因子认证数据库设计                            #
#                                                                         #
###########################################################################


CREATE DATABASE IF NOT EXISTS `cm` CHARACTER SET utf8;

USE `cm`;

CREATE TABLE IF NOT EXISTS `two_factor_email` (
  `id` INT(21) NOT NULL AUTO_INCREMENT,
  `mailserver` VARCHAR(128) NOT NULL DEFAULT '' COMMENT '服务器地址',
  `tls` INT(21) NOT NULL DEFAULT 0 COMMENT '0 不使用加密 | 1 使用加密',
  `port` INT(21) NOT NULL DEFAULT 25 COMMENT '邮件服务器端口号',
  `mailfrom` VARCHAR(128) NOT NULL DEFAULT '' COMMENT '发件箱',
  `password` VARCHAR(128) NOT NULL DEFAULT '' COMMENT '发件箱密码',
  `extra` VARCHAR(1024) NOT NULL DEFAULT '{ }' COMMENT '附加字段, 字段扩展',
  PRIMARY KEY pk_id  (id)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb3;

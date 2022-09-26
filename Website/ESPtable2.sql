-- phpMyAdmin SQL Dump
-- version 4.6.6deb4
-- https://www.phpmyadmin.net/
--
-- Servidor: db5000128127.hosting-data.io
-- Tiempo de generación: 18-07-2019 a las 16:50:19
-- Versión del servidor: 5.7.25-log
-- Versión de PHP: 7.0.33-0+deb9u3

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- Base de datos: `dbs122647`
--

-- --------------------------------------------------------

--
-- Estructura de tabla para la tabla `ESPtable2`
--

CREATE TABLE `ESPtable2` (
  `id` int(5) NOT NULL,
  `PASSWORD` char(15) NOT NULL,
  `CURRENT_T` decimal(4,1) NOT NULL,
  `BOOL_ON_OFF` tinyint(1) NOT NULL,
  `BOOL_OPEN_CLOSE` int(1) NOT NULL,
  `BOOL_SIDE` int(1) NOT NULL,
  `BOOL_AUTO` int(1) NOT NULL,
  `TARGET_T` int(3) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Volcado de datos para la tabla `ESPtable2`
--

INSERT INTO `ESPtable2` (`id`, `PASSWORD`, `CURRENT_T`, `BOOL_ON_OFF`, `BOOL_OPEN_CLOSE`, `BOOL_SIDE`, `BOOL_AUTO`, `TARGET_T`) VALUES
(99999, 'Password123456-', 12.3, 0, 1, 1, 1, 20);

--
-- Índices para tablas volcadas
--

--
-- Indices de la tabla `ESPtable2`
--
ALTER TABLE `ESPtable2`
  ADD PRIMARY KEY (`id`);

--
-- AUTO_INCREMENT de las tablas volcadas
--

--
-- AUTO_INCREMENT de la tabla `ESPtable2`
--
ALTER TABLE `ESPtable2`
  MODIFY `id` int(5) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=100000;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;

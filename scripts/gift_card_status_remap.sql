-- Переназначение статусов подарочных карт (склады / r_store_acc / документы не меняем)
--   9  -> 12
--   3  -> 6
--  14  -> 15
--
-- d_gift_cart.f_status и справочник d_gift_cart_statuses.
-- Сначала PREVIEW, затем APPLY в транзакции.

/* ========== PREVIEW: карты ========== */

SELECT f_status AS old_status, COUNT(*) AS cnt
FROM d_gift_cart
WHERE f_status IN (9, 3, 14)
GROUP BY f_status
ORDER BY f_status;

SELECT c.f_id, c.f_code, c.f_status AS old_status,
       CASE c.f_status
           WHEN 9 THEN 12
           WHEN 3 THEN 6
           WHEN 14 THEN 15
       END AS new_status
FROM d_gift_cart c
WHERE c.f_status IN (9, 3, 14)
ORDER BY c.f_status, c.f_code;

/* ========== PREVIEW: справочник статусов ========== */

SELECT f_id, f_name
FROM d_gift_cart_statuses
WHERE f_id IN (3, 6, 9, 12, 14, 15)
ORDER BY f_id;

SELECT old_id, new_id,
       (SELECT f_name FROM d_gift_cart_statuses s WHERE s.f_id = m.old_id) AS old_name,
       (SELECT f_name FROM d_gift_cart_statuses s WHERE s.f_id = m.new_id) AS new_name,
       (SELECT COUNT(*) FROM d_gift_cart_statuses s WHERE s.f_id = m.new_id) AS new_id_exists
FROM (
    SELECT 9 AS old_id, 12 AS new_id
    UNION ALL SELECT 3, 6
    UNION ALL SELECT 14, 15
) m
ORDER BY old_id;

/* ========== APPLY ========== */

/*
START TRANSACTION;

-- 1) карты
UPDATE d_gift_cart
SET f_status = CASE f_status
        WHEN 9 THEN 12
        WHEN 3 THEN 6
        WHEN 14 THEN 15
        ELSE f_status
    END
WHERE f_status IN (9, 3, 14);

-- 2) справочник: перенос f_id, если целевой id ещё не занят (MySQL-safe)
UPDATE d_gift_cart_statuses s
INNER JOIN (
    SELECT 9 AS old_id, 12 AS new_id
    UNION ALL SELECT 3, 6
    UNION ALL SELECT 14, 15
) m ON s.f_id = m.old_id
SET s.f_id = m.new_id
WHERE NOT EXISTS (
    SELECT 1 FROM (SELECT f_id FROM d_gift_cart_statuses) t WHERE t.f_id = m.new_id
);

-- 3) дубликаты старых id (целевой статус уже был) — удалить
DELETE FROM d_gift_cart_statuses
WHERE f_id IN (9, 3, 14);

-- 4) при необходимости добавить целевые статусы (имя со склада, r_store не меняем)
INSERT INTO d_gift_cart_statuses (f_id, f_name)
SELECT r.f_id, r.f_name
FROM r_store r
WHERE r.f_id IN (6, 12, 15)
  AND r.f_state = 1
  AND NOT EXISTS (
      SELECT 1 FROM d_gift_cart_statuses g WHERE g.f_id = r.f_id
  );

-- проверка
SELECT f_status, COUNT(*) AS cnt
FROM d_gift_cart
WHERE f_status IN (3, 6, 9, 12, 14, 15)
GROUP BY f_status
ORDER BY f_status;

SELECT f_id, f_name
FROM d_gift_cart_statuses
WHERE f_id IN (3, 6, 9, 12, 14, 15)
ORDER BY f_id;

-- COMMIT;
-- ROLLBACK;
*/

-- Корректировка: приход на склад 27 по картам d_gift_cart.f_status = 27
-- (1 шт. номинала на карту, материал по f_initialamount — как GiftCartStore::dishForAmount)
--
-- Перед применением: выполните блок PREVIEW. При INSERT — START TRANSACTION и проверьте суммы.
-- f_op: подставьте id пользователя или оставьте 0.

/* ========== PREVIEW ========== */
SELECT
    ROUND(c.f_initialamount) AS nominal,
    COUNT(*) AS card_qty,
    CASE ROUND(c.f_initialamount)
        WHEN 10000 THEN 468
        WHEN 20000 THEN 450
        WHEN 25000 THEN 472
        WHEN 30000 THEN 451
        WHEN 40000 THEN 452
        WHEN 50000 THEN 453
        ELSE 0
    END AS f_material,
    d.f_lastprice,
    COUNT(*) * COALESCE(d.f_lastprice, 0) AS line_total
FROM d_gift_cart c
LEFT JOIN r_dish d ON d.f_id = CASE ROUND(c.f_initialamount)
        WHEN 10000 THEN 468
        WHEN 20000 THEN 450
        WHEN 25000 THEN 472
        WHEN 30000 THEN 451
        WHEN 40000 THEN 452
        WHEN 50000 THEN 453
        ELSE 0
    END
WHERE c.f_status = 27
GROUP BY ROUND(c.f_initialamount), d.f_lastprice
ORDER BY nominal;

SELECT c.f_id, c.f_code, c.f_initialamount, c.f_status
FROM d_gift_cart c
WHERE c.f_status = 27
  AND CASE ROUND(c.f_initialamount)
        WHEN 10000 THEN 468
        WHEN 20000 THEN 450
        WHEN 25000 THEN 472
        WHEN 30000 THEN 451
        WHEN 40000 THEN 452
        WHEN 50000 THEN 453
        ELSE 0
    END = 0;

/* ========== APPLY (раскомментируйте после проверки PREVIEW) ========== */

/*
START TRANSACTION;

SET @store_id := 27;
SET @doc_type := 1;
SET @doc_state := 1;
SET @doc_date := CURDATE();
SET @doc_op := 0;
SET @remarks := 'Correction IN: gift cards f_status=27';

INSERT INTO r_docs (
    f_date, f_type, f_state, f_partner, f_inv, f_invDate,
    f_amount, f_remarks, f_op, f_fullDate, f_payment
)
VALUES (
    @doc_date, @doc_type, @doc_state, 0, '', NULL,
    0, @remarks, @doc_op, NOW(), 1
);

SET @doc_id := LAST_INSERT_ID();

INSERT INTO r_body (
    f_doc, f_store, f_material, f_sign, f_qty, f_price, f_total, f_vat
)
SELECT
    @doc_id,
    @store_id,
    g.f_material,
    1,
    g.card_qty,
    g.f_lastprice,
    g.card_qty * g.f_lastprice,
    0
FROM (
    SELECT
        CASE ROUND(c.f_initialamount)
            WHEN 10000 THEN 468
            WHEN 20000 THEN 450
            WHEN 25000 THEN 472
            WHEN 30000 THEN 451
            WHEN 40000 THEN 452
            WHEN 50000 THEN 453
            ELSE 0
        END AS f_material,
        COUNT(*) AS card_qty,
        d.f_lastprice
    FROM d_gift_cart c
    INNER JOIN r_dish d ON d.f_id = CASE ROUND(c.f_initialamount)
            WHEN 10000 THEN 468
            WHEN 20000 THEN 450
            WHEN 25000 THEN 472
            WHEN 30000 THEN 451
            WHEN 40000 THEN 452
            WHEN 50000 THEN 453
            ELSE 0
        END
    WHERE c.f_status = 27
    GROUP BY f_material, d.f_lastprice
    HAVING f_material > 0
) g;

INSERT INTO r_store_acc (
    f_doc, f_docrow, f_base, f_store, f_goods, f_qty, f_price, f_sign
)
SELECT
    b.f_doc, b.f_id, b.f_id, b.f_store, b.f_material, b.f_qty, b.f_price, b.f_sign
FROM r_body b
WHERE b.f_doc = @doc_id;

UPDATE r_docs d
SET d.f_amount = (
    SELECT COALESCE(SUM(b.f_total), 0)
    FROM r_body b
    WHERE b.f_doc = @doc_id
)
WHERE d.f_id = @doc_id;

SELECT @doc_id AS created_doc_id, d.f_amount, d.f_remarks
FROM r_docs d
WHERE d.f_id = @doc_id;

SELECT b.f_id, b.f_material, b.f_qty, b.f_price, b.f_total
FROM r_body b
WHERE b.f_doc = @doc_id;

-- COMMIT;
-- ROLLBACK;
*/

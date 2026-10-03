import zipfile
import xml.etree.ElementTree as ET

path = r'e:\download\էԼԻՏ ԿՈԴԵՐ (1).xlsx'
out = r'c:\Development\projects\elite.carwash\talon_excel_unsold.sql'
out_fix = r'c:\Development\projects\elite.carwash\talon_excel_fix.sql'
NS = '{http://schemas.openxmlformats.org/spreadsheetml/2006/main}'
RID = '{http://schemas.openxmlformats.org/officeDocument/2006/relationships}id'

with zipfile.ZipFile(path) as z:
    wb = ET.fromstring(z.read('xl/workbook.xml'))
    rels = ET.fromstring(z.read('xl/_rels/workbook.xml.rels'))
    rel_ns = '{http://schemas.openxmlformats.org/package/2006/relationships}'
    rid_to_target = {r.attrib['Id']: r.attrib['Target'] for r in rels.findall(rel_ns + 'Relationship')}
    shared = []
    if 'xl/sharedStrings.xml' in z.namelist():
        sst = ET.fromstring(z.read('xl/sharedStrings.xml'))
        for si in sst.findall(NS + 'si'):
            texts = si.findall('.//' + NS + 't')
            shared.append(''.join((t.text or '') for t in texts))

    def cell_val(c):
        t = c.get('t')
        v = c.find(NS + 'v')
        if v is None or v.text is None:
            return ''
        if t == 's':
            return shared[int(v.text)]
        return v.text

    rows = []
    for sh in wb.find(NS + 'sheets').findall(NS + 'sheet'):
        g = int(sh.attrib['name'])
        target = 'xl/' + rid_to_target[sh.attrib[RID]].replace('xl/', '')
        root = ET.fromstring(z.read(target))
        for row in root.findall('.//' + NS + 'row'):
            for c in row.findall(NS + 'c'):
                val = cell_val(c).strip()
                if val:
                    rows.append((g, val.replace("'", "''")))

lines = [
    '-- Generated from Excel: e:\\download\\էԼԻՏ ԿՈԴԵՐ (1).xlsx',
    '-- Groups: 1(120), 3(10), 4(30), 11(108), 12(24), 14(15), 23(50) = 357 codes',
    'START TRANSACTION;',
    '',
    'DROP TEMPORARY TABLE IF EXISTS talon_excel_unsold;',
    'CREATE TEMPORARY TABLE talon_excel_unsold (',
    '  f_group INT NOT NULL,',
    '  f_code  VARCHAR(32) NOT NULL,',
    '  PRIMARY KEY (f_code)',
    ');',
    '',
    'INSERT INTO talon_excel_unsold (f_group, f_code) VALUES',
]
for i, (g, code) in enumerate(rows):
    comma = ',' if i < len(rows) - 1 else ';'
    lines.append(f"  ({g}, '{code}'){comma}")

lines += [
    '',
    '-- 1) Сравнение: непроданных сейчас vs Excel',
    'SELECT',
    '  g.f_id AS f_group,',
    '  g.f_name,',
    '  COALESCE(cur.unsold_cnt, 0) AS unsold_now,',
    '  COALESCE(xl.cnt, 0) AS unsold_excel,',
    '  COALESCE(cur.unsold_cnt, 0) - COALESCE(xl.cnt, 0) AS diff',
    'FROM talon_service_items_group g',
    'LEFT JOIN (',
    '  SELECT f_group, COUNT(*) unsold_cnt',
    '  FROM talon_service',
    '  WHERE COALESCE(f_trsale, 0) = 0',
    '  GROUP BY f_group',
    ') cur ON cur.f_group = g.f_id',
    'LEFT JOIN (',
    '  SELECT f_group, COUNT(*) cnt FROM talon_excel_unsold GROUP BY f_group',
    ') xl ON xl.f_group = g.f_id',
    'WHERE g.f_id BETWEEN 1 AND 24',
    'ORDER BY g.f_id;',
    '',
    '-- 2) Коды из Excel, которых нет в talon_service',
    'SELECT e.f_group, e.f_code',
    'FROM talon_excel_unsold e',
    'LEFT JOIN talon_service t ON t.f_code = e.f_code',
    'WHERE t.f_id IS NULL',
    'ORDER BY e.f_group, e.f_code;',
    '',
    '-- 3) Будут помечены -33: все в группах 1..24, кроме кодов из Excel',
    'SELECT t.f_group, COUNT(*) AS cnt_to_mark_33',
    'FROM talon_service t',
    'LEFT JOIN talon_excel_unsold e ON e.f_code = t.f_code',
    'WHERE t.f_group BETWEEN 1 AND 24',
    '  AND e.f_code IS NULL',
    'GROUP BY t.f_group',
    'ORDER BY t.f_group;',
    '',
    '-- 4) Коды из Excel в другой группе или уже проданы',
    'SELECT e.f_group AS excel_group, t.f_group AS db_group, e.f_code, t.f_trsale',
    'FROM talon_excel_unsold e',
    'INNER JOIN talon_service t ON t.f_code = e.f_code',
    'WHERE t.f_group <> e.f_group OR COALESCE(t.f_trsale, 0) <> 0',
    'ORDER BY e.f_group, e.f_code;',
    '',
    '-- ===== FIX: -33 = корректировка (продано + использовано) =====',
    'SET @correction_doc = -33;',
    '',
    '-- 5a) Добавить отсутствующие коды из Excel как непроданные',
    'INSERT INTO talon_service (f_code, f_group, f_trregister, f_trsale, f_trback, f_used)',
    'SELECT e.f_code, e.f_group, 1, NULL, NULL, 0',
    'FROM talon_excel_unsold e',
    'LEFT JOIN talon_service t ON t.f_code = e.f_code',
    'WHERE t.f_id IS NULL;',
    '',
    '-- 5b) Коды из Excel -> непроданные (только если нет реальной продажи: f_trsale <= 0)',
    'UPDATE talon_service t',
    'INNER JOIN talon_excel_unsold e ON e.f_code = t.f_code',
    'SET t.f_group = e.f_group,',
    '    t.f_trregister = 1,',
    '    t.f_trsale = NULL,',
    '    t.f_trback = NULL,',
    '    t.f_used = 0,',
    '    t.f_order = NULL',
    'WHERE COALESCE(t.f_trsale, 0) <= 0;',
    '',
    '-- 5c) Остальные 1..24, не из Excel, без реальной продажи -> doc -33',
    'UPDATE talon_service t',
    'LEFT JOIN talon_excel_unsold e ON e.f_code = t.f_code',
    'SET t.f_trregister = 1,',
    '    t.f_trsale = @correction_doc,',
    '    t.f_trback = @correction_doc,',
    '    t.f_used = 1',
    'WHERE t.f_group BETWEEN 1 AND 24',
    '  AND e.f_code IS NULL',
    '  AND COALESCE(t.f_trsale, 0) <= 0;',
    '',
    '-- 6) Проверка: непроданных должно быть как в Excel',
    'SELECT',
    '  g.f_id AS f_group,',
    '  COALESCE(cur.unsold_cnt, 0) AS unsold_now,',
    '  COALESCE(xl.cnt, 0) AS unsold_excel,',
    '  COALESCE(cur.unsold_cnt, 0) - COALESCE(xl.cnt, 0) AS diff',
    'FROM talon_service_items_group g',
    'LEFT JOIN (',
    '  SELECT f_group, COUNT(*) unsold_cnt',
    '  FROM talon_service',
    '  WHERE COALESCE(f_trsale, 0) = 0',
    '  GROUP BY f_group',
    ') cur ON cur.f_group = g.f_id',
    'LEFT JOIN (',
    '  SELECT f_group, COUNT(*) cnt FROM talon_excel_unsold GROUP BY f_group',
    ') xl ON xl.f_group = g.f_id',
    'WHERE g.f_id BETWEEN 1 AND 24',
    'ORDER BY g.f_id;',
    '',
    '-- 7) Группы 1..24 без листа в Excel: непроданных быть не должно',
    'SELECT t.f_group, COUNT(*) AS unsold_cnt',
    'FROM talon_service t',
    'WHERE t.f_group BETWEEN 1 AND 24',
    '  AND COALESCE(t.f_trsale, 0) = 0',
    '  AND t.f_group NOT IN (SELECT DISTINCT f_group FROM talon_excel_unsold)',
    'GROUP BY t.f_group',
    'ORDER BY t.f_group;',
    '',
    '-- COMMIT;',
    '-- ROLLBACK;',
]

with open(out, 'w', encoding='utf-8') as f:
    f.write('\n'.join(lines))

fix_lines = [
    '-- FIX ONLY: сверка непроданных с Excel',
    '-- Реальная продажа (f_trsale > 0) НЕ трогается',
    '-- Excel: e:\\download\\էԼԻՏ ԿՈԴԵՐ (1).xlsx',
    'START TRANSACTION;',
    '',
    'DROP TEMPORARY TABLE IF EXISTS talon_excel_unsold;',
    'CREATE TEMPORARY TABLE talon_excel_unsold (',
    '  f_group INT NOT NULL,',
    '  f_code  VARCHAR(32) NOT NULL,',
    '  PRIMARY KEY (f_code)',
    ');',
    '',
    'INSERT INTO talon_excel_unsold (f_group, f_code) VALUES',
]
for i, (g, code) in enumerate(rows):
    comma = ',' if i < len(rows) - 1 else ';'
    fix_lines.append(f"  ({g}, '{code}'){comma}")

fix_lines += [
    '',
    '-- Документы корректировки (отчёт считает только с JOIN на header!)',
    'INSERT INTO talon_documents_header (f_id, f_type, f_date, f_user, f_partner, f_amount)',
    'SELECT 1, 1, \'2020-01-01\', 0, 0, 0 FROM DUAL',
    'WHERE NOT EXISTS (SELECT 1 FROM talon_documents_header WHERE f_id = 1);',
    '',
    'SET @correction_doc = -33;',
    '',
    'INSERT INTO talon_documents_header (f_id, f_type, f_date, f_user, f_partner, f_amount)',
    'SELECT @correction_doc, 2, \'2020-01-01\', 0, 0, 0 FROM DUAL',
    'WHERE NOT EXISTS (SELECT 1 FROM talon_documents_header WHERE f_id = @correction_doc);',
    '',
    'INSERT INTO talon_service (f_code, f_group, f_trregister, f_trsale, f_trback, f_used)',
    'SELECT e.f_code, e.f_group, 1, NULL, NULL, 0',
    'FROM talon_excel_unsold e',
    'LEFT JOIN talon_service t ON t.f_code = e.f_code',
    'WHERE t.f_id IS NULL;',
    '',
    '-- Excel: зарегистрирован (doc 1), не продан -> остаток +1 в отчёте',
    'UPDATE talon_service t',
    'INNER JOIN talon_excel_unsold e ON e.f_code = t.f_code',
    'SET t.f_group = e.f_group,',
    '    t.f_trregister = 1,',
    '    t.f_trsale = NULL,',
    '    t.f_trback = NULL,',
    '    t.f_used = 0,',
    '    t.f_order = NULL',
    'WHERE COALESCE(t.f_trsale, 0) <= 0;',
    '',
    '-- Остальные 1..24, без реальной продажи -> doc -33, остаток 0 в отчёте',
    'UPDATE talon_service t',
    'LEFT JOIN talon_excel_unsold e ON e.f_code = t.f_code',
    'SET t.f_trregister = 1,',
    '    t.f_trsale = @correction_doc,',
    '    t.f_trback = @correction_doc,',
    '    t.f_used = 1',
    'WHERE t.f_group BETWEEN 1 AND 24',
    '  AND e.f_code IS NULL',
    '  AND COALESCE(t.f_trsale, 0) <= 0;',
    '',
    'COMMIT;',
]

with open(out_fix, 'w', encoding='utf-8') as f:
    f.write('\n'.join(fix_lines))

print(f'written {out}, codes={len(rows)}')
print(f'written {out_fix}')

#include "pdfgen.h"
#include "stdio.h"
#include <string.h>
#include <algorithm>

#include "web_server.h"
#include "event_log.h"


const uint8_t logo_png[] = {
  0x89, 0x50, 0x4e, 0x47, 0x0d, 0x0a, 0x1a, 0x0a, 0x00, 0x00, 0x00, 0x0d,
  0x49, 0x48, 0x44, 0x52, 0x00, 0x00, 0x01, 0x0d, 0x00, 0x00, 0x00, 0x23,
  0x01, 0x03, 0x00, 0x00, 0x00, 0xb4, 0x0d, 0xcd, 0xb9, 0x00, 0x00, 0x00,
  0x06, 0x50, 0x4c, 0x54, 0x45, 0xf0, 0x1f, 0x56, 0xff, 0xff, 0xff, 0xbc,
  0x9d, 0xd6, 0xe9, 0x00, 0x00, 0x00, 0x01, 0x74, 0x52, 0x4e, 0x53, 0x00,
  0x40, 0xe6, 0xd8, 0x66, 0x00, 0x00, 0x00, 0x4d, 0x49, 0x44, 0x41, 0x54,
  0x38, 0xcb, 0x63, 0x78, 0xc0, 0xc0, 0xc0, 0xce, 0xc0, 0xc0, 0x50, 0xc0,
  0xfe, 0x1f, 0x02, 0x0e, 0xc8, 0x43, 0x19, 0x0c, 0xf5, 0x10, 0xfa, 0x07,
  0x03, 0x75, 0x95, 0x30, 0x40, 0x00, 0xe3, 0x01, 0x28, 0x83, 0x1d, 0x4a,
  0x33, 0x58, 0x8c, 0x2a, 0x19, 0x42, 0x4a, 0xe8, 0x96, 0x5e, 0xd8, 0x61,
  0x6e, 0x91, 0x81, 0x39, 0x02, 0xe6, 0xbc, 0xe1, 0xa2, 0x84, 0x1d, 0x45,
  0x09, 0x72, 0x80, 0x0f, 0x03, 0x25, 0xd0, 0x78, 0xfd, 0x40, 0x6b, 0x25,
  0x00, 0xcd, 0xa5, 0xe8, 0x0e, 0xf6, 0x3c, 0x2d, 0x03, 0x00, 0x00, 0x00,
  0x00, 0x49, 0x45, 0x4e, 0x44, 0xae, 0x42, 0x60, 0x82
};
unsigned int logo_png_len = 165;

#define LEFT_MARGIN PDF_MM_TO_POINT(15)
#define RIGHT_MARGIN PDF_MM_TO_POINT(10)
// TODO: This is the bottom margin of the logo background rectangle!
// That is not what one expects to be the top margin of the page
#define TOP_MARGIN PDF_MM_TO_POINT(35)
#define BOTTOM_MARGIN PDF_MM_TO_POINT(10)

#define LINE_WIDTH (PDF_A4_WIDTH - LEFT_MARGIN - RIGHT_MARGIN)
#define FONT_SIZE 9
#define LINE_SPACING 4
#define LINE_HEIGHT (FONT_SIZE + LINE_SPACING)
#define LOGO_BACKGROUND 0xFF545454
/*
const char * stats = "Wallbox: wallbox:innen\0"
                     "Exportiert am 31.01.2023\0"
                     "Exportierter Zeitraum: Aufzeichnungsbeginn - 31.01.2023\0"
                     "Gesamt geladene Energie: 7959.926 kWh\0"
                     "Gesamtbetrag: 2053.66 €";
*/

const char * letterhead = "Herr von und zu Testuser\0"
                          "Teststraße 123\0"
                          "45678 Teststadt\0"
                          "Testland";
#define LETTERHEAD_LINES 4

#define DISPLAY_NAME_COLUMN 1
const char * table_header = "Startzeit\0"
                            "Anzeigename\0"
                            "geladen (kWh)\0"
                            "Ladedauer\0"
                            "Zählerstand Start\0"
                            "Kosten (€)";

#define TABLE_HEADER_COLS 6
/*
const char * const table_line[] {
    "08.04.2022 06:10",
    "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@",
    "123,456",
    "1002:42:40",
    "6.123.909,848",
    "999,99"
};

const char * const table_lines[] {
    "08.04.2022 06:10", "ÆÆÆÆÆÆÆÆÆÆÆÆÆÆÆÆÆÆÆÆÆÆÆÆÆÆÆÆÆÆÆÆ", "123,456", "1002:42:40", "6.123.909,848", "999,99",
    "09.04.2022 06:10", "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@", "123,456", "1002:42:40", "6.123.909,848", "999,99",
    "10.04.2022 06:10", "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@", "123,456", "1002:42:40", "6.123.909,848", "999,99",
    "11.04.2022 06:10", "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@", "123,456", "1002:42:40", "6.123.909,848", "999,99",
    "12.04.2022 06:10", "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@", "123,456", "1002:42:40", "6.123.909,848", "999,99",
    "13.04.2022 06:10", "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@", "123,456", "1002:42:40", "6.123.909,848", "999,99",
    "14.04.2022 06:10", "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@", "123,456", "1002:42:40", "6.123.909,848", "999,99",
    "11.04.2022 06:10", "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@", "123,456", "1002:42:40", "6.123.909,848", "999,99",
};
*/

float table_column_offsets[] {
    (0 * (LINE_WIDTH / TABLE_HEADER_COLS)),
    (0.8 * (LINE_WIDTH / TABLE_HEADER_COLS)),
    (3.4 * (LINE_WIDTH / TABLE_HEADER_COLS)),
    (4.125 * (LINE_WIDTH / TABLE_HEADER_COLS)),
    (4.75 * (LINE_WIDTH / TABLE_HEADER_COLS)),
    (5.575 * (LINE_WIDTH / TABLE_HEADER_COLS)),
    LINE_WIDTH
};

#define TABLE_LINES_FIRST_PAGE 32
#define TABLE_LINES_PER_PAGE 40

#define TABLE_LINES_PER_OBJECT 8

int get_streams_per_page(bool first_page, int *table_lines_to_place) {
    int result = 0;
    if (first_page) {
        result += 1  // letter head
                + 1; // stats
    }
    result += 1 // table header
            + 1  // logo background
            + 1; // page number

    int to_place = std::min(*table_lines_to_place, first_page ? TABLE_LINES_FIRST_PAGE : TABLE_LINES_PER_PAGE);
    *table_lines_to_place -= to_place;

    result += ceil((float)to_place / TABLE_LINES_PER_OBJECT) // table content
            + 1;    // line borders

    return result;
}

int init_pdf_generator(WebServerRequest *request, const char *stats, int stats_lines, uint16_t tracked_charges, std::function<int(const char * *)> table_lines_cb) {
    struct pdf_info info = {
        /*.creator =*/ "My software",
        /*.producer =*/ "My software",
        /*.title =*/ "My document",
        /*.author =*/ "My name",
        /*.subject =*/ "My subject",
        /*.date =*/ "Today"
    };

    struct pdf_doc *pdf = pdf_create(PDF_A4_WIDTH, PDF_A4_HEIGHT, &info);
    pdf_add_write_callback(pdf, [request](const void *buf, size_t len) -> int {
        int rc = request->sendChunk((const char *)buf, len);
        if (rc != ESP_OK)
            return -abs(rc);

        return len;
    });
    int pages_created = 0;
    int pages_to_be_created = 0;
    int table_lines_last_page = 0;

    int table_content_placed = 0;
    int table_lines_to_place = 0;


    //printf("pdf_set_font %d %s\n", pdf_set_font(pdf, DEFAULT_FONT), pdf_get_err(pdf, NULL));

    table_lines_to_place = tracked_charges;

    while (table_lines_to_place > 0) {
        table_lines_last_page = table_lines_to_place;
        int streams = get_streams_per_page(pages_to_be_created == 0, &table_lines_to_place);
        //printf("pdf_append_page %p %s\n", pdf_append_page(pdf, streams, 1), pdf_get_err(pdf, NULL));
        pdf_notify_page(pdf, streams, 1);
        ++pages_to_be_created;
    }

    table_lines_to_place = tracked_charges;

    pdf_add_page_callback(pdf, [&table_lines_last_page, &pages_created, &table_lines_to_place](struct pdf_doc *pdf, uint32_t page_num) -> int {
        table_lines_last_page = table_lines_to_place;
        int streams = get_streams_per_page(pages_created == 0, &table_lines_to_place);
        pdf_append_page(pdf, streams, 1);
        ++pages_created;
        return 0;
    });

    pdf_add_image_callback(pdf, [](struct pdf_doc *pdf, uint32_t page_num, uint32_t image_num) -> int {
        return pdf_add_png_image_data(pdf, NULL, LEFT_MARGIN, PDF_A4_HEIGHT - 100 + 18, -1, 75 - 18 * 2, LOGO_BACKGROUND, logo_png, logo_png_len);
    });


    pdf_add_stream_callback(pdf, [pages_to_be_created, table_lines_last_page, &table_content_placed, tracked_charges, table_lines_cb, stats, stats_lines](struct pdf_doc *pdf, uint32_t page_num, uint32_t stream_num) -> int {
        // Logo background
        if (stream_num == 0)
            return pdf_add_filled_rectangle(pdf, NULL, 0, PDF_A4_HEIGHT - TOP_MARGIN, PDF_A4_WIDTH, 75, 0, LOGO_BACKGROUND, 0);
        --stream_num;

        // Page number
        if (stream_num == 0) {
            float width = 0.0f;
            char buf[32] = {};
            snprintf(buf, ARRAY_SIZE(buf), "Seite %d von %d", page_num + 1, pages_to_be_created);
            pdf_get_font_text_width(pdf, DEFAULT_FONT, buf, FONT_SIZE, &width);
            return pdf_add_text(pdf, NULL, buf, FONT_SIZE, (PDF_A4_WIDTH - width) / 2, BOTTOM_MARGIN, PDF_BLACK);
        }
        --stream_num;

        // First page
        if (page_num == 0) {
            // Stats block (top right)
            if (stream_num == 0) {
                float offsets[2] = {0, LINE_WIDTH};
                return pdf_add_multiple_text_spacing(pdf, NULL, stats, stats_lines, 1, FONT_SIZE, LEFT_MARGIN + table_column_offsets[2], PDF_A4_HEIGHT - TOP_MARGIN - 10 - (LINE_HEIGHT * 1), PDF_BLACK, 0, LINE_HEIGHT, offsets);
            }
            --stream_num;

            // Letter head (top left)
            if (stream_num == 0) {
                float offsets[2] = {0, LINE_WIDTH};
                return pdf_add_multiple_text_spacing(pdf, NULL, letterhead, 4, 1, FONT_SIZE, LEFT_MARGIN, PDF_A4_HEIGHT - TOP_MARGIN - 10 - (LINE_HEIGHT * (stream_num + 1)), PDF_BLACK, 0, LINE_HEIGHT, offsets);
            }
            --stream_num;
        }

        auto content_offset = PDF_A4_HEIGHT - TOP_MARGIN - 10 - (page_num == 0 ? (LINE_HEIGHT * (stats_lines + 2)) : LINE_HEIGHT);

        // Table header
        if (stream_num == 0) {
            return pdf_add_multiple_text_spacing(pdf, NULL, table_header, 1, 6, FONT_SIZE, LEFT_MARGIN, content_offset, PDF_BLACK, 0, LINE_HEIGHT, table_column_offsets, false);
        }
        --stream_num;

        { // Table lines
            auto table_line_offset = content_offset - (LINE_HEIGHT * 1.2 * (stream_num + 1)) - LINE_HEIGHT * 0.3;
            auto table_lines = (page_num + 1) == pages_to_be_created ? table_lines_last_page : (page_num == 0 ? TABLE_LINES_FIRST_PAGE : TABLE_LINES_PER_PAGE);

            if (stream_num == 0) {
                return pdf_add_horizontal_lines(pdf, nullptr, LEFT_MARGIN, table_line_offset, PDF_A4_WIDTH - RIGHT_MARGIN, table_line_offset, 0.5, PDF_BLACK, LINE_HEIGHT * 1.2, table_lines, true);
            }

            --stream_num;
        }

        // Table content
        auto table_text_offset = content_offset - (LINE_HEIGHT * 1.2 * (1 + ((int)(stream_num * TABLE_LINES_PER_OBJECT))));

        auto lines = std::min(8, tracked_charges - table_content_placed);
        table_content_placed += lines;

        const char *lines_string;
        int lines_generated = table_lines_cb(&lines_string);
        // TODO: check if lines_generated != lines and if so handle this somehow.
        (void) lines_generated;

        return pdf_add_multiple_text_spacing(pdf, NULL, lines_string, lines, 6, FONT_SIZE, LEFT_MARGIN, table_text_offset, PDF_BLACK, 0, LINE_HEIGHT * 1.2, table_column_offsets);
    });

    pdf_save_file(pdf);
    pdf_destroy(pdf);

    return 0;
}
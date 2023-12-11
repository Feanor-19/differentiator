#ifndef DIFF_OUTPUT_H
#define DIFF_OUTPUT_H

#include "differentiator_common.h"


const char * const LATEX_SECTION_GIVEN      = "Дано:";
const char * const LATEX_SECTION_DIFF       = "Продифффффференцируем:";
const char * const LATEX_SECTION_SIMPLIFY   = "Упростим:";


DiffStatus diff_print_expr_as_text( FILE *stream, const Expression *expr_ptr );

//! @brief Creates .tex file with this name
//! and writes latex header into it. If there is already latex file
//! started (and not yet ended), returns error and does nothing.
DiffStatus diff_start_latex_doc( const char *dir_name );

//! @brief Writes given expression into ALREADY CREATED
//! AND OPENED USING diff_start_latex_doc() .tex file. If no .tex file
//! is currently started, returns error and does nothing.
DiffStatus diff_write_latex_expr( const Expression *expr_ptr );

DiffStatus diff_write_latex_rand_phrase( );

DiffStatus diff_write_latex_section( const char *section_name );

//! @brief Ends currently opened latex doc file and creates pdf.
//! If no .tex file is currently started, returns error and does nothing.
DiffStatus diff_end_latex_doc();

#endif

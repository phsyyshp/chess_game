def file_rank_to_row_column(position_file_rank):
    return (int(position_file_rank[1]) - 1, ord(position_file_rank[0]) - 97)


def row_column_to_file_rank(position_row_column: list):
    return chr(position_row_column[1] + 97) + str(position_row_column[0] + 1)


def combine_old_new_row_column_to_rank_file(
    source_row_column,
    destination_row_column,
):
    return row_column_to_file_rank(source_row_column) + row_column_to_file_rank(
        destination_row_column
    )


def split_single_file_rank_to_old_new_row_column(old_new_file_rank):
    old_position_file_rank = old_new_file_rank[0:2]
    new_position_file_rank = old_new_file_rank[2:4]
    return file_rank_to_row_column(old_position_file_rank), file_rank_to_row_column(
        new_position_file_rank
    )

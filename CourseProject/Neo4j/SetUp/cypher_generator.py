from pathlib import Path
import glob
from conf import FILE_PATH, CYPHER_FILE, NEO4J_CONF, CONSTRAINT_QUERIES, VIDEOS_LOAD_QUERY, CATEGORY_LOAD_QUERY, \
    USER_LOAD_QUERY, \
    RELATED_VIDEO_LOAD_QUERY, COMMAND, RELATED_VIDEO_LOAD_APOC_QUERY

print("Base Path: ", Path(FILE_PATH), "\n")


# Function to create cypher queries
def get_cypher_queries_for_file(file_name):
    return ' '.join([VIDEOS_LOAD_QUERY % file_name, CATEGORY_LOAD_QUERY % file_name, USER_LOAD_QUERY % file_name])


def get_connect_video_query(filename):
    return RELATED_VIDEO_LOAD_QUERY % filename

def get_connect_video_apoc_query(filenames):
    return RELATED_VIDEO_LOAD_APOC_QUERY % filenames

# command to execute cypher
def get_command(f):
    file_name = Path(f.name).absolute()
    print('\nCypher Script Path: ', file_name)
    return "cat  '%s' | %s" % (file_name, COMMAND)


# Get all the files from the subdirectories of base directory
sub_dir_all_files = glob.glob(FILE_PATH)

# filter lot.txt files which are not going use
selected_files = filter(lambda file_name: not file_name.endswith('log.txt'), sub_dir_all_files)

# create the absolute path as URI for each file
selected_files_uri_format = list(map(lambda file_name: Path(file_name).absolute().as_uri(), selected_files))

# Create New Cypher Script
with open(CYPHER_FILE, 'w') as f:
    # Write the constraint queries in the start of a file
    f.write(CONSTRAINT_QUERIES)

    # Generate Load-Video-User-Category cypher queries for each file and write to cypher script
    # for file in selected_files_uri_format:
    #     # print("Writing Queries for File: ", file)
    #     queries = get_cypher_queries_for_file(file)
    #     f.write(queries)
    #     # break

    # Generate Connect-Video cypher queries for each file and write to cypher script
    # for file in selected_files_uri_format:
    #     # print("Writing Queries for File: ", file)
    #     queries = get_connect_video_query(file)
    #     f.write(queries)
    #     # break

    apoc_query = get_connect_video_apoc_query(selected_files_uri_format)
    f.write(apoc_query)

    # Get the command to run the above cypher script0
    command = get_command(f)

print("\nSuccess, Run following command from Neo4j Desktop Terminal to load the data: \n%s\n" % command)

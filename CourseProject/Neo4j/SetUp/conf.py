from sys import platform

# Base directory of the files
BASE_DIR = '/Users/linhnguyen/Desktop/youtubeData'
FILE_PATH = BASE_DIR + '/*/*/*.*'

CYPHER_FILE = 'DataLoad.cypher'
# Neo4j configurations
NEO4J_CONF = {
    "HOSTNAME": "localhost:7687",
    "USERNAME": "neo4j",
    "PASSWORD": "3"
}

CONSTRAINT_QUERIES = '''
CREATE CONSTRAINT ON (c:Category) ASSERT c.name IS UNIQUE;
CREATE CONSTRAINT ON (v:Video) ASSERT v.id IS UNIQUE;
CREATE CONSTRAINT ON (u:User) ASSERT u.username IS UNIQUE;
'''

VIDEOS_LOAD_QUERY = '''
USING PERIODIC COMMIT
LOAD CSV FROM '%s' AS line FIELDTERMINATOR '\\t'
WITH line
WHERE NOT line[0] IS NULL
MERGE (v:Video {id:line[0]})
SET 
    v.age=toInteger(line[2]),
    v.length=toInteger(line[4]),
    v.views=toInteger(line[5]),
    v.rate=toFloat(line[6]),
    v.ratings=toInteger(line[7]),
    v.comments=toInteger(line[8]);
'''

CATEGORY_LOAD_QUERY = '''
USING PERIODIC COMMIT
LOAD CSV FROM '%s' AS line FIELDTERMINATOR '\\t'
WITH line
WHERE NOT line[3] IS NULL
MATCH (v:Video {id: line[0]})
MERGE (c:Category {name:line[3]})
MERGE (v)-[:HAS_CATEGORY]-(c);
'''

USER_LOAD_QUERY = '''
USING PERIODIC COMMIT
LOAD CSV FROM '%s' AS line FIELDTERMINATOR '\\t'
WITH line
WHERE NOT line[1] IS NULL
MATCH (v:Video {id: line[0]})
MERGE (u:User {username:line[1]})
MERGE (u)-[:UPLOADED]-(v);
'''

RELATED_VIDEO_LOAD_QUERY = '''
USING PERIODIC COMMIT
LOAD CSV FROM '%s' AS line FIELDTERMINATOR '\\t'
WITH line
WHERE NOT line[0] IS NULL
MATCH (current_video:Video {id:line[0]})
FOREACH (video_id in [ id in line[9..28] WHERE id IS NOT NULL] | MERGE (related_video:Video {id: video_id}) MERGE (current_video)-[:RELATED_TO]->(related_video));
'''

RELATED_VIDEO_LOAD_APOC_QUERY = '''
CALL apoc.periodic.iterate(
  "
    WITH %s AS filenames
    UNWIND filenames AS file
    RETURN file;
  ",
  "
    LOAD CSV FROM {file} AS line FIELDTERMINATOR '\\t'
    WITH line
    WHERE NOT line[0] IS NULL
    MATCH (current_video:Video {id:line[0]}) 
    FOREACH (video_id in [ id in line[9..28] WHERE id IS NOT NULL] | MERGE (related_video:Video {id: video_id}) MERGE (current_video)-[:RELATED_TO]->(related_video));
    ",{parallel:false,iterateList:true});
'''


PROGRAM = "cypher-shell"
if platform == "linux" or platform == "linux2" or platform == "darwin":
    PROGRAM = "./cypher-shell"

COMMAND = "%s -a %s -u %s -p %s --format verbose" % \
          (PROGRAM, NEO4J_CONF['HOSTNAME'], NEO4J_CONF['USERNAME'], NEO4J_CONF['PASSWORD'])

# COMMAND = f" {PROGRAM} -a {NEO4J_CONF['HOSTNAME']} -u {NEO4J_CONF['USERNAME']} -p {NEO4J_CONF['PASSWORD']}" \
#           f" --format verbose"

wget http://api.sen.se/events/?sense_key=API_KEY \
--post-data '[{"feed_id": 'FEED_ID',"value": 'VALUE'}]' \
--header 'Content-Type: application/json' -O - 

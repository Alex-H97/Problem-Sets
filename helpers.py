import feedparser
import urllib.parse
# import time


def lookup(geo):
    """Look up articles for geo"""

    # Check cache
    try:
        if geo in lookup.cache:
            return lookup.cache[geo]
    except AttributeError:
        lookup.cache = {}

    # Replace special characters
    escaped = urllib.parse.quote(geo, safe="")

    # Get feed from Google
    feed = feedparser.parse(f"https://news.google.com/news/rss/local/section/geo/{escaped}")

    # If no items in feed, get feed from Onion
    if not feed["items"]:
        feed = feedparser.parse("http://www.theonion.com/feeds/rss")

    # Cache results
    lookup.cache[geo] = [{"link": item["link"], "title": item["title"],
                          "published": item["published_parsed"]}
                         for item in feed["items"]]

    # Sort results by date (newest to oldest)
    lookup.cache[geo] = sorted(lookup.cache[geo],
                               key=lambda item: item["published"],
                               reverse=True)

    # Return results
    return lookup.cache[geo]
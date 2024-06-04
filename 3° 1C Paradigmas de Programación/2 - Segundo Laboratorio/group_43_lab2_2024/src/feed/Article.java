package feed;

public class Article {
	private String title;
	private String description;
	private String pubDate;
	private String link;

	public Article(String title, String description, String pubDate, String link) {
		this.title = title;
		this.description = description;
		this.pubDate = pubDate;
		this.link = link;
	}

	public String getTitle() {
		return title;
	}

	public void setTitle(String title) {
		this.title = title;
	}

	public String getDescription() {
		return description;
	}

	public void setDescription(String description) {
		this.description = description;
	}

	public String getPubDate() {
		return pubDate;
	}

	public void setPubDate(String pubDate) {
		this.pubDate = pubDate;
	}

	public String getLink() {
		return link;
	}

	public void setLink(String link) {
		this.link = link;
	}

	public void printPretty() {
		System.out.println(
				"**********************************************************************************************");
		System.out.println("Title: " + getTitle());
		System.out.println("Description: " + getDescription());
		System.out.println("Publication Date: " + getPubDate());
		System.out.println("Link: " + getLink());
		System.out.println("********************************************************************************");
	}
}
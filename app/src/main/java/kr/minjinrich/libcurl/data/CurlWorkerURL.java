package kr.minjinrich.libcurl.data;

public class CurlWorkerURL
{
	private String url;
	private int count;

	public CurlWorkerURL( String url )
	{
		this.url = url;
		this.count = 0;
	}

	public String getUrl()
	{
		return this.url;
	}

	public void setUrl( String url )
	{
		this.url = url;
	}

	public int getCount()
	{
		return this.count;
	}

	public void setCount( int count )
	{
		this.count = count;
	}
}

class Movie{
    private String title,year,genre[]=new String[3],time,company,budget,revenue;

    public Movie(String line){
        String[] strarr=line.split(",");

        title=strarr[0];
        year=strarr[1];
        genre[0]=strarr[2];
        genre[1]=strarr[3];
        genre[2]=strarr[4];
        time=strarr[5];
        company=strarr[6];
        budget=strarr[7];
        revenue=strarr[8];
    }

    public String getTitle() {
        return title;
    }

    public String getYear() {
        return year;
    }

    public String[] getGenre() {
        return genre;
    }

    public String getTime() {
        return time;
    }

    public String getCompany() {
        return company;
    }

    public String getBudget() {
        return budget;
    }

    public String getRevenue() {
        return revenue;
    }

    public void setTitle(String title) {
        this.title = title;
    }

    public void setYear(String year) {
        this.year = year;
    }

    public void setGenre(String[] genre) {
        this.genre = genre;
    }

    public void setTime(String time) {
        this.time = time;
    }

    public void setCompany(String company) {
        this.company = company;
    }

    public void setBudget(String budget) {
        this.budget = budget;
    }

    public void setRevenue(String revenue) {
        this.revenue = revenue;
    }
}

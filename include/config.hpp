class Config {
    public:
        Config(string filename);
        ~Config();
        vector<int> getPids();
    private:
        void parse(string filename);
        vector<int> pids;
}

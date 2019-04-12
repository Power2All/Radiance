#ifndef RADIANCE_DB_H
#define RADIANCE_DB_H
#pragma GCC visibility push(default)
#include <mysql++/mysql++.h>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <mutex>

class dbConnectionPool : public mysqlpp::ConnectionPool {
	private:
		void load_config();
		unsigned int mysql_port, mysql_connections, mysql_timeout;
		std::string mysql_db, mysql_host, mysql_username, mysql_password;
		std::unordered_set<mysqlpp::Connection*> in_use_connections;
		std::mutex pool_lock;

	public:
		dbConnectionPool();
		~dbConnectionPool();
		mysqlpp::Connection* grab();
		mysqlpp::Connection* exchange(const mysqlpp::Connection* conn);
		mysqlpp::Connection* exchange(const mysqlpp::ScopedConnection* conn);
		void release(const mysqlpp::Connection* conn);
		void release(const mysqlpp::ScopedConnection* conn);

	protected:
		mysqlpp::Connection* create();
		void destroy(mysqlpp::Connection* conn);
		unsigned int max_idle_time();
};

class database {
	private:
		dbConnectionPool* pool;
		std::string update_user_buffer;
		std::string update_torrent_buffer;
		std::string update_peer_heavy_buffer;
		std::string update_peer_light_buffer;
		std::string update_peer_hist_buffer;
		std::string update_snatch_buffer;
		std::string update_token_buffer;

		std::queue<std::string> user_queue;
		std::queue<std::string> torrent_queue;
		std::queue<std::string> peer_queue;
		std::queue<std::string> peer_hist_queue;
		std::queue<std::string> snatch_queue;
		std::queue<std::string> token_queue;

		bool u_active, t_active, p_active, s_active, h_active, tok_active;
		bool readonly, load_peerlists, clear_peerlists, peers_history, snatched_history, files_peers;

        // Database 'torrents'
        std::string db_torrents;
        std::string db_torrents_id;
        std::string db_torrents_info_hash;
        std::string db_torrents_freetorrent;
        std::string db_torrents_doubletorrent;
        std::string db_torrents_snatched;
        std::string db_torrents_seeders;
        std::string db_torrents_leechers;
        std::string db_torrents_balance;
        std::string db_torrents_last_action;

        // Database 'users_main'
        std::string db_users_main;
        std::string db_users_main_id;
        std::string db_users_main_can_leech;
        std::string db_users_main_torrent_pass;
        std::string db_users_main_visible;
        std::string db_users_main_track_ipv6;
        std::string db_users_main_personal_freeleech;
        std::string db_users_main_personal_doubleseed;
        std::string db_users_main_enabled;
        std::string db_users_main_uploaded;
        std::string db_users_main_downloaded;
        std::string db_users_main_uploaded_daily;
        std::string db_users_main_downloaded_daily;

        // Database 'users_slots'
        std::string db_users_slots;
        std::string db_users_slots_user_id;
        std::string db_users_slots_free_leech;
        std::string db_users_slots_double_seed;
        std::string db_users_slots_torrent_id;

        // Database 'users_freeleeches'
        std::string db_users_freeleeches;
        std::string db_users_freeleeches_user_id;
        std::string db_users_freeleeches_torrent_id;
        std::string db_users_freeleeches_downloaded;
        std::string db_users_freeleeches_uploaded;

        // Database 'xbt_files_users'
        std::string db_xbt_files_users;
        std::string db_xbt_files_users_peer_id;
        std::string db_xbt_files_users_port;
        std::string db_xbt_files_users_ipv4;
        std::string db_xbt_files_users_ipv6;
        std::string db_xbt_files_users_uploaded;
        std::string db_xbt_files_users_downloaded;
        std::string db_xbt_files_users_remaining;
        std::string db_xbt_files_users_corrupt;
        std::string db_xbt_files_users_announced;
        std::string db_xbt_files_users_ctime;
        std::string db_xbt_files_users_mtime;
        std::string db_xbt_files_users_uid;
        std::string db_xbt_files_users_active;
        std::string db_xbt_files_users_fid;
        std::string db_xbt_files_users_upspeed;
        std::string db_xbt_files_users_downspeed;
        std::string db_xbt_files_users_timespent;
        std::string db_xbt_files_users_useragent;

        // Database 'xbt_client_blacklist'
        std::string db_xbt_client_blacklist;
        std::string db_xbt_client_blacklist_peer_id;

        // Database 'xbt_snatched'
        std::string db_xbt_snatched;
        std::string db_xbt_snatched_uid;
        std::string db_xbt_snatched_fid;
        std::string db_xbt_snatched_tstamp;
        std::string db_xbt_snatched_ipv4;
        std::string db_xbt_snatched_ipv6;

        // Database 'xbt_peers_history'
        std::string db_xbt_peers_history;
        std::string db_xbt_peers_history_uid;
        std::string db_xbt_peers_history_downloaded;
        std::string db_xbt_peers_history_remaining;
        std::string db_xbt_peers_history_uploaded;
        std::string db_xbt_peers_history_upspeed;
        std::string db_xbt_peers_history_downspeed;
        std::string db_xbt_peers_history_timespent;
        std::string db_xbt_peers_history_peer_id;
        std::string db_xbt_peers_history_ipv4;
        std::string db_xbt_peers_history_ipv6;
        std::string db_xbt_peers_history_fid;
        std::string db_xbt_peers_history_mtime;

		// These locks prevent more than one thread from reading/writing the buffers.
		// These should be held for the minimum time possible.
		std::mutex user_queue_lock;
		std::mutex torrent_buffer_lock;
		std::mutex torrent_queue_lock;
		std::mutex peer_queue_lock;
		std::mutex peer_hist_queue_lock;
		std::mutex snatch_queue_lock;
		std::mutex token_queue_lock;

		void load_config();


		void flush_users();
		void flush_torrents();
		void flush_snatches();
		void flush_peers();
		void flush_peer_hist();
		void flush_tokens();
		void do_flush(bool &active, std::queue<std::string> &queue, std::mutex &lock, std::atomic<uint64_t> &queue_size, const std::string queue_name);
		void clear_peer_data();

		peer_list::iterator add_peer(peer_list &peer_list, const std::string &peer_id);
		static inline bool peer_is_visible(user_ptr &u, peer *p);

	public:
		database();
		void shutdown();
		void reload_config();
		void load_site_options();
		void load_torrents(torrent_list &torrents);
		void load_tokens(torrent_list &torrents);
		void load_users(user_list &users);
		void load_peers(torrent_list &torrents, user_list &users);
		void load_seeders(torrent_list &torrents, user_list &users);
		void load_leechers(torrent_list &torrents, user_list &users);
		void load_blacklist(std::vector<std::string> &blacklist);

		void record_user(const std::string &record); // (id,uploaded_change,downloaded_change)
		void record_torrent(const std::string &record); // (id,seeders,leechers,snatched_change,balance)
		void record_snatch(const std::string &record, const std::string &ipv4, const std::string &ipv6); // (uid,fid,tstamp)
		void record_peer(const std::string &record, const std::string &ipv4, const std::string &ipv6, int port, const std::string &peer_id, const std::string &useragent); // (uid,fid,active,peerid,useragent,ip,port,uploaded,downloaded,upspeed,downspeed,left,timespent,announces,tstamp)
		void record_peer(const std::string &record, const std::string &peer_id); // (fid,peerid,timespent,announces,tstamp)
		void record_peer_hist(const std::string &record, const std::string &peer_id, const std::string &ipv4, const std::string &ipv6, int tid);
		void record_token(const std::string &record);

		void flush();
		bool all_clear();

		std::mutex torrent_list_mutex;
		std::mutex user_list_mutex;
		std::mutex domain_list_mutex;
		std::mutex blacklist_mutex;
};

#pragma GCC visibility pop
#endif

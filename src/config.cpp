#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include "config.h"
#include "misc_functions.h"

confval::confval() :
	bool_val(false),
	uint_val(0),
	str_val(""),
	val_type(CONF_NONEXISTENT) {}

confval::confval(bool value) :
	bool_val(value),
	uint_val(0),
	str_val(""),
	val_type(CONF_BOOL) {}

confval::confval(unsigned int value) :
	bool_val(false),
	uint_val(value),
	str_val(""),
	val_type(CONF_UINT) {}

confval::confval(const char * value) :
	bool_val(false),
	uint_val(0),
	str_val(value),
	val_type(CONF_STR) {}

const bool confval::get_bool() {
	return bool_val;
}

const unsigned int confval::get_uint() {
	return uint_val;
}

const time_t confval::get_time() {
	return time_t(uint_val);
}

const std::string confval::get_str() {
	return str_val;
}

void confval::set(const std::string &value) {
	if (val_type == CONF_BOOL) {
		bool_val = value == "1" || value == "true" || value == "yes";
	} else if (val_type == CONF_UINT) {
		uint_val = strtoint32(value);
	} else if (val_type == CONF_STR) {
		str_val = value;
	}
}

settings::settings() {
	init();
	dummy_setting = new confval(); // Safety value to use if we're accessing nonexistent settings
}
settings::~settings() {
	delete dummy_setting;
}

options::options() {
	init();
	dummy_setting = new confval(); // Safety value to use if we're accessing nonexistent settings
}
options::~options() {
	delete dummy_setting;
}

void settings::init() {
	// Internal stuff
	add("listen_port", 2710u);
	add("listen_host", "0.0.0.0");
	add("listen_path", "");
	add("max_connections", 1024u);
	add("max_middlemen", 20000u);
	add("max_read_buffer", 4096u);
	add("connection_timeout", 10u);
	add("keepalive_timeout", 0u);

	// Tracker requests
	add("announce_interval", 1800u);
	add("max_request_size", 4096u);
	add("numwant_limit", 50u);

	// Timers
	add("del_reason_lifetime", 86400u);
	add("peers_timeout", 7200u);
	add("reap_peers_interval", 1800u);
	add("schedule_interval", 3u);

	// MySQL
	add("mysql_db", "gazelle");
	add("mysql_host", "localhost");
	add("mysql_port", 3306u);
	add("mysql_path", "");
	add("mysql_username", "");
	add("mysql_password", "");
	add("mysql_connections", 8u);
	add("mysql_timeout", 30u);

	// Site communication
	add("site_host", "127.0.0.1");
	add("site_port", 80u);
	add("site_path", "");
	add("site_password",   "00000000000000000000000000000000");
	add("report_password", "00000000000000000000000000000000");

	// General Control
	add("readonly", false);
	add("anonymous", false);
	add("anonymous_password", "00000000000000000000000000000000");
	add("clear_peerlists", true);
	add("load_peerlists", false);
	add("peers_history", true);
	add("files_peers", true);
	add("snatched_history", true);
	add("daemonize", false);
	add("syslog_path", "off");
	add("syslog_level", "info");
	add("pid_file", "./radiance.pid");
	add("daemon_user", "root");

	// Database Structure 'torrents'
    add("db_torrents", "torrents");
    add("db_torrents_id", "ID");
    add("db_torrents_info_hash", "info_hash");
    add("db_torrents_freetorrent", "freetorrent");
    add("db_torrents_doubletorrent", "doubletorrent");
    add("db_torrents_snatched", "Snatched");
    add("db_torrents_seeders", "Seeders");
    add("db_torrents_leechers", "Leechers");
    add("db_torrents_balance", "Balance");
    add("db_torrents_last_action", "last_action");

    // Database Structure 'users_main'
    add("db_users_main", "users_main");
    add("db_users_main_id", "ID");
    add("db_users_main_can_leech", "can_leech");
    add("db_users_main_torrent_pass", "torrent_pass");
    add("db_users_main_visible", "Visible");
    add("db_users_main_track_ipv6", "track_ipv6");
    add("db_users_main_personal_freeleech", "personal_freeleech");
    add("db_users_main_personal_doubleseed", "personal_doubleseed");
    add("db_users_main_enabled", "Enabled");
    add("db_users_main_uploaded", "Uploaded");
    add("db_users_main_downloaded", "Downloaded");
    add("db_users_main_uploaded_daily", "UploadedDaily");
    add("db_users_main_downloaded_daily", "DownloadedDaily");

    // Database Structure 'users_slots'
    add("db_users_slots", "users_slots");
    add("db_users_slots_user_id", "UserID");
    add("db_users_slots_free_leech", "FreeLeech");
    add("db_users_slots_double_seed", "DoubleSeed");
    add("db_users_slots_torrent_id", "TorrentID");

    // Database Structure 'users_freeleeches'
    add("db_users_freeleeches", "users_freeleeches");
    add("db_users_freeleeches_user_id", "UserID");
    add("db_users_freeleeches_torrent_id", "TorrentID");
    add("db_users_freeleeches_downloaded", "Downloaded");
    add("db_users_freeleeches_uploaded", "Uploaded");

    // Database Structure 'xbt_files_users'
    add("db_xbt_files_users", "xbt_files_users");
    add("db_xbt_files_users_peer_id", "peer_id");
    add("db_xbt_files_users_port", "port");
    add("db_xbt_files_users_ipv4", "ipv4");
    add("db_xbt_files_users_ipv6", "ipv6");
    add("db_xbt_files_users_uploaded", "uploaded");
    add("db_xbt_files_users_downloaded", "downloaded");
    add("db_xbt_files_users_remaining", "remaining");
    add("db_xbt_files_users_corrupt", "corrupt");
    add("db_xbt_files_users_announced", "announced");
    add("db_xbt_files_users_ctime", "ctime");
    add("db_xbt_files_users_mtime", "mtime");
    add("db_xbt_files_users_uid", "uid");
    add("db_xbt_files_users_active", "active");
    add("db_xbt_files_users_fid", "fid");
    add("db_xbt_files_users_upspeed", "upspeed");
    add("db_xbt_files_users_downspeed", "downspeed");
    add("db_xbt_files_users_timespend", "timespend");
    add("db_xbt_files_users_useragent", "useragent");

    // Database Structure 'xbt_client_blacklist'
    add("db_xbt_client_blacklist", "xbt_client_blacklist");
    add("db_xbt_client_blacklist_peer_id", "peer_id");

    // Database Structure 'xbt_snatched'
    add("db_xbt_snatched", "xbt_snatched");
    add("db_xbt_snatched_uid", "uid");
    add("db_xbt_snatched_fid", "fid");
    add("db_xbt_snatched_tstamp", "tstamp");
    add("db_xbt_snatched_ipv4", "ipv4");
    add("db_xbt_snatched_ipv6", "ipv6");

    // Database Structure 'xbt_peers_history'
    add("db_xbt_peers_history", "xbt_peers_history");
    add("db_xbt_peers_history_uid", "uid");
    add("db_xbt_peers_history_downloaded", "downloaded");
    add("db_xbt_peers_history_remaining", "remaining");
    add("db_xbt_peers_history_uploaded", "uploaded");
    add("db_xbt_peers_history_upspeed", "upspeed");
    add("db_xbt_peers_history_downspeed", "downspeed");
    add("db_xbt_peers_history_timespent", "timespent");
    add("db_xbt_peers_history_peer_id", "peer_id");
    add("db_xbt_peers_history_ipv4", "ipv4");
    add("db_xbt_peers_history_ipv6", "ipv6");
    add("db_xbt_peers_history_fid", "fid");
    add("db_xbt_peers_history_mtime", "mtime");}

void options::init() {
	add("SitewideFreeleechMode", "off");
	add("SitewideFreeleechStartTime", 0u);
	add("SitewideFreeleechEndTime", 0u);
	add("SitewideDoubleseedMode", "off");
	add("SitewideDoubleseedStartTime", 0u);
	add("SitewideDoubleseedEndTime", 0u);
	add("EnableIPv6Tracker", false);
}

confval * config::get(const std::string &setting_name) {
	const auto setting = settings.find(setting_name);
	if (setting == settings.end()) {
		std::cerr << "WARNING: Unrecognized setting '" << setting_name << "'" << std::endl;
		return dummy_setting;
	}
	return &setting->second;
}

bool config::get_bool(const std::string &setting_name) {
	return get(setting_name)->get_bool();
}

unsigned int config::get_uint(const std::string &setting_name) {
	return get(setting_name)->get_uint();
}

std::string config::get_str(const std::string &setting_name) {
	return get(setting_name)->get_str();
}

time_t config::get_time(const std::string &setting_name) {
	return get(setting_name)->get_time();
}

void config::set(const std::string &section_name, const std::string &setting_name, const std::string &value) {
	if (section_name != "tracker") return;
	get(setting_name)->set(value);
}

void settings::load(const std::string &conf_file_path, std::istream &conf_file) {
	load(conf_file);
	add("conf_file_path", conf_file_path.c_str());
}

void settings::load(std::istream &conf_file) {
	std::string line;
	std::string section = "global";
	while (getline(conf_file, line)) {
		line = trim(line);
		size_t pos;
		if (line[0] != '#' && line[0] != ';') {
			if (line[0] == '[' && (pos = line.find(']')) != std::string::npos) {
				section = trim(line.substr(1, pos-1));
			} else if ((pos = line.find('=')) != std::string::npos) {
				std::string key(trim(line.substr(0, pos)));
				std::string value(trim(line.substr(pos + 1)));
				set(section, key, value);
			}
		}
	}
}

void settings::reload() {
	const std::string conf_file_path(get_str("conf_file_path"));
	std::ifstream conf_file(conf_file_path);
	if (conf_file.fail()) {
		std::cerr << "Config file '" << conf_file_path << "' couldn't be opened" << std::endl;
	} else {
		init();
		load(conf_file);
	}
}

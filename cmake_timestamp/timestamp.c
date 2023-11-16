#define DEFINE_ZBSTR(var,str) const struct {unsigned char len; char content[sizeof(str)];} (var) = {sizeof(str)-1, (str)}

//DEFINE_ZBSTR(git_hash_str,GIT_HASH);
DEFINE_ZBSTR(git_short_hash_str, GIT_SHORT_HASH);
//DEFINE_ZBSTR(git_branch_str, GIT_BRANCH);
//DEFINE_ZBSTR(git_dirty_str, GIT_DIRTY);
DEFINE_ZBSTR(build_time_str, BUILD_TIME);
//DEFINE_ZBSTR(cpack_version_str, CPACK_VERSION);

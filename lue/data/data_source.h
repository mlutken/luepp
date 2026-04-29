#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include <variant>
#include <functional>

// ------------------------
// --- data_object_base ---
// ------------------------

/// @todo Make real class using std::filesystem::path internally. Must support network paths as well.
using data_path = std::string;

using data_changed_cb = std::function<void (const data_path&)>;

struct data_object_base
{
    using id_t = size_t;
    virtual ~data_object_base() = default;
    virtual id_t            id          () const = 0;
    virtual std::string     to_string   () const = 0;
};

using data_object_sp = std::shared_ptr<data_object_base>;

// ----------------
// --- my_point ---
// ----------------

struct my_point : public data_object_base
{
    my_point() = default;
    ~my_point() override = default;
    my_point(const my_point&) = default;
    my_point(my_point&&) = default;
    my_point& operator=(const my_point&) = default;
    my_point& operator=(my_point&&) = default;

    explicit my_point(uint32_t xval, uint32_t yval) : x(xval), y(yval) {}

    uint32_t x  {};
    uint32_t y  {};
    id_t            id          () const override;
    std::string     to_string   () const override;

};

// ------------------
// --- data_value ---
// ------------------
class data_source_base;
using data_source_base_sp   = std::shared_ptr<data_source_base>;
using data_value            = std::variant<int32_t, int64_t, float, double, bool, std::string,
                                           data_object_sp, data_source_base_sp>;
using data_value_vec        = std::vector<data_value>;

std::string     to_string   (const data_value& val);
std::string     to_string   (const data_value_vec& val);

// ------------------------
// --- data_source_base ---
// ------------------------

/**
@todo Most likely we do not want data_source_base to be copyable ...
 */
class data_source_base
{
public:
    data_source_base()                                      = default;
    virtual ~data_source_base()                             = default;
    data_source_base(const data_source_base&)               = default;
    data_source_base(data_source_base&&)                    = default;
    data_source_base& operator=(const data_source_base&)    = default;
    data_source_base& operator=(data_source_base&&)         = default;
    explicit data_source_base   (data_path);


    void                        set                 (const data_path& path, data_value val);
    void                        set                 (const data_path& path, data_value_vec val);
    void                        set_data_source     (const data_path& path, data_source_base_sp val);

    int32_t                     as_int32            (const data_path& path) const;
    const std::string&          as_string           (const data_path& path)  const;
    const data_value&           as_data_value       (const data_path& path)  const;

    void                        connect             (data_changed_cb cb);

    bool                        is_read_only        () const;

    const data_path&            path                () const    { return path_; }
    std::string                 to_string           () const;
    void                        dbg_print           () const;

protected:
    virtual const data_value&   do_as_data_value    (const data_path& path)  const = 0;
    virtual void                do_set              (const data_path& path, data_value val) = 0;
    virtual void                do_set_data_vec     (const data_path& path, data_value_vec val) = 0;
    virtual void                do_set_data_source  (const data_path& path, data_source_base_sp val) = 0;
    virtual std::string         do_to_string        () const = 0;

    virtual bool                do_is_read_only     () const { return false; }

private:
    using data_changed_cb_vec_t = std::vector<data_changed_cb>;

    void                        call_on_changed     (const data_path& path) const;

    data_path                   path_                   {};
    data_changed_cb_vec_t       data_changed_cb_vec_    {};
};


class data_source : public data_source_base
{
public:
    using data_source_base::data_source_base;
    data_source() = default;


protected:
    const data_value&           do_as_data_value    (const data_path& path)  const override;
    void                        do_set              (const data_path& path, data_value val) override;
    void                        do_set_data_vec     (const data_path& path, data_value_vec val) override;
    void                        do_set_data_source  (const data_path& path, data_source_base_sp val) override;

    std::string                 do_to_string       () const override;

private:
    using map_string_data_t     = std::unordered_map<std::string, data_value>;
    using map_string_vec_t      = std::unordered_map<std::string, data_value_vec>;

    map_string_data_t           string_data_map_        {};
    map_string_vec_t            string_vec_map_         {};
};

// ----------------------------------------------
// --- data_source playground and adhoc tests ---
// ----------------------------------------------

void        data_source_playground_1();
data_source create_demo_1           ();

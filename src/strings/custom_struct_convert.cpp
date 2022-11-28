

static int64_t custom_get(winrt::event_token const& instance)
{
    return instance.value;
}

static void custom_set(winrt::event_token& instance, int64_t value)
{
    instance.value = value;
}

static int32_t custom_get(winrt::hresult const& instance)
{
    return instance;
}

static void custom_set(winrt::hresult& instance, int32_t value)
{
    instance = value;
}
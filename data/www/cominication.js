(() => {
  function getQueryParam(param) {
    const urlParams = new URLSearchParams(window.location.search);
    return urlParams.get(param);
  }

  const username = getQueryParam('username');
  const fallbackUsername = 'gekie';

  window.UserData = {
    username: username || fallbackUsername,
    data: null,
    ready: false,
    onReadyCallbacks: [],
    onReady: function (cb) {
      if (this.ready) {
        cb(this.data);
      } else {
        this.onReadyCallbacks.push(cb);
      }
    }
  };

  if (username) {
    fetch(`/get-data?username=${encodeURIComponent(username)}`)
      .then(response => {
        if (!response.ok) throw new Error('Ophalen mislukt');
        return response.json();
      })
      .then(json => {
        window.UserData.data = json;
        window.UserData.ready = true;
        window.UserData.onReadyCallbacks.forEach(cb => cb(json));
      })
      .catch(err => {
        console.error("Fout bij ophalen data:", err);
      });
    e = document.getElementById("username");
    e.value = username;
  }
})();

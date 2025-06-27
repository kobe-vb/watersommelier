function getQueryParam(param) {
  const urlParams = new URLSearchParams(window.location.search);
  return urlParams.get(param);
}

const username = getQueryParam('username');

console.log(username);
if (username) {
  document.getElementById('userId').textContent = username;
} else {
  document.getElementById('userId').textContent = 'gekie';
}
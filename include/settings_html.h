#pragma once

const char settings_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
  <head>
    <meta charset="UTF-8" />
    <meta name="viewport" content="width=device-width, initial-scale=1.0" />
    <title>WiFi Settings</title>
    <style>
      html,
      body {
        margin: 0;
        padding: 0.5rem;
        font-family: sans-serif;
        --accent-color: #006aff;
        --transparent-accent-color: color-mix(
          in srgb,
          var(--accent-color),
          transparent 90%
        );
      }
      @media screen and (max-width: 389px) {
        html,
        body {
          font-size: 14px;
        }
      }
      html *,
      body * {
        box-sizing: border-box;
      }
      html *:focus,
      html *:focus-visible,
      body *:focus,
      body *:focus-visible {
        outline-color: var(--accent-color);
      }

      .sr-only {
        position: absolute;
        width: 1px;
        height: 1px;
        margin: -1px;
        padding: 0;
        overflow: hidden;
        clip: rect(0, 0, 0, 0);
        white-space: nowrap;
        border: 0;
      }

      *:not(select) > button {
        --button-color: var(--accent-color);
        --background-color: var(--transparent-accent-color);
        appearance: none;
        cursor: pointer;
        border: 2px solid var(--button-color);
        color: var(--button-color);
        font-weight: bold;
        text-transform: uppercase;
        font-size: 0.8em;
        padding: 0.1em 0.7em 0;
        min-height: 1.8em;
        border-radius: 0.9em;
        display: inline-flex;
        align-items: center;
        justify-content: center;
        background-color: var(--background-color);
        white-space: nowrap;
      }
      *:not(select) > button:focus,
      *:not(select) > button:focus-visible {
        outline: 1px auto;
        outline-offset: 3px;
      }
      *:not(select) > button.secondaryButton {
        --button-color: inherit;
        --background-color: transparent;
      }
      *:not(select) > button.textButton {
        border: none;
        font-weight: inherit;
        text-transform: inherit;
        font-size: inherit;
        padding: 0;
        min-height: auto;
        border-radius: 0;
        --background-color: transparent;
        text-decoration: underline;
      }
      *:not(select) > button.textButton:focus,
      *:not(select) > button.textButton:focus-visible {
        outline-offset: 0;
      }

      .iconWrapper {
        display: inline-flex;
      }
      .iconWrapper > span {
        display: inline-flex;
      }
      .iconWrapper > span > img {
        width: 1.4em;
      }

      .formField {
        margin-bottom: 0.8em;
      }
      .formField label {
        display: block;
        font-size: 0.9em;
        margin-bottom: 0.2em;
      }
      .formField select,
      .formField input {
        width: 100%;
        max-width: 300px;
        padding: 0.2em 0.4em;
        font-size: inherit;
        border-radius: 0.5em;
        border: 1px solid;
        border-color: inherit;
        background-color: white;
      }
      .formField select:disabled,
      .formField input:disabled {
        background-color: lightgray;
      }
      .formField select,
      .formField select::picker(select) {
        appearance: base-select;
      }
      .formField select::picker(select) {
        margin-top: 0.2em;
        border-radius: 0.5em;
        border: 1px solid;
        border-color: inherit;
      }

      a,
      a:link,
      a:visited,
      a:active {
        color: inherit;
      }

      main {
        max-width: 900px;
        margin: 0 auto;
      }

      h1,
      h2 {
        margin: 0 0 0.8rem;
      }
      h1 + p,
      h2 + p {
        margin-top: -0.8rem;
        margin-bottom: 0.8rem;
        font-size: 0.9rem;
        font-style: italic;
      }

      .currentNetwork,
      .availableNetworks {
        background-color: var(--transparent-accent-color);
      }

      .currentNetwork,
      .currentAccessPoint,
      .availableNetworks {
        border: 1px solid var(--accent-color);
        padding: 1rem;
        margin-bottom: 1rem;
      }
      .currentNetwork details,
      .currentAccessPoint details,
      .availableNetworks details {
        margin-top: 1rem;
        font-size: 0.9em;
      }
      .currentNetwork details summary,
      .currentAccessPoint details summary,
      .availableNetworks details summary {
        cursor: pointer;
      }
      .currentNetwork details > div,
      .currentAccessPoint details > div,
      .availableNetworks details > div {
        margin-top: 0.5em;
      }
      .currentNetwork dl,
      .currentAccessPoint dl,
      .availableNetworks dl {
        margin: 0;
      }
      .currentNetwork dl > div dt,
      .currentAccessPoint dl > div dt,
      .availableNetworks dl > div dt {
        font-weight: bold;
        display: inline-block;
        margin-right: 0.5em;
        width: 5.5em;
      }
      .currentNetwork dl > div dd,
      .currentAccessPoint dl > div dd,
      .availableNetworks dl > div dd {
        display: inline-block;
        margin: 0;
      }
      .currentNetwork .card,
      .currentAccessPoint .card,
      .availableNetworks .card {
        background-color: var(--transparent-accent-color);
        padding: 0 0.5em 0 1em;
        font-size: 0.9em;
        display: flex;
        align-items: center;
        justify-content: space-between;
        height: 2.4em;
        border-radius: 1.2em;
      }
      .currentNetwork .card dl > div dt,
      .currentAccessPoint .card dl > div dt,
      .availableNetworks .card dl > div dt {
        width: auto;
      }

      .currentNetwork .currentNetworkCard > div {
        display: flex;
        flex-grow: 1;
        overflow: hidden;
        padding-right: 1.2em;
        align-items: center;
      }
      .currentNetwork .currentNetworkCard > div #currentNetworkSSID {
        order: 2;
        flex-grow: 1;
        overflow: hidden;
        text-overflow: ellipsis;
        font-weight: bold;
      }
      .currentNetwork .currentNetworkCard > div #currentNetworkStrength {
        order: 1;
      }
      .currentNetwork .currentNetworkCard > div #currentNetworkStrength,
      .currentNetwork .currentNetworkCard > div #currentNetworkSecure {
        margin-right: 0.9em;
      }

      .availableNetworks
        .availableNetworksInner
        .availableNetworksManuallyInsertWrapper {
        font-size: 0.9em;
      }
      .availableNetworks .availableNetworksInner select {
        max-width: 400px;
      }
      .availableNetworks .availableNetworksInner select::picker(select) {
        max-width: 100%;
      }
      .availableNetworks
        .availableNetworksInner
        select
        selectedcontent
        .networkStrength,
      .availableNetworks
        .availableNetworksInner
        select
        selectedcontent
        .networkSecure {
        display: none;
      }
      .availableNetworks .availableNetworksInner select option {
        display: none;
      }
      .availableNetworks
        .availableNetworksInner
        select
        #availableNetworksOptions
        option {
        display: flex;
        padding: 0.8em;
        overflow: hidden;
      }
      .availableNetworks
        .availableNetworksInner
        select
        #availableNetworksOptions
        option::checkmark {
        display: none;
      }
      .availableNetworks
        .availableNetworksInner
        select
        #availableNetworksOptions
        option
        .networkSSID {
        order: 3;
        flex-grow: 1;
        overflow: hidden;
        text-overflow: ellipsis;
        font-weight: bold;
      }
      .availableNetworks
        .availableNetworksInner
        select
        #availableNetworksOptions
        option
        .networkStrength {
        order: 1;
      }
      .availableNetworks
        .availableNetworksInner
        select
        #availableNetworksOptions
        option
        .networkSecure {
        order: 2;
      }

      #setNetworkDialog::backdrop {
        background-color: rgba(0, 0, 0, 0.5);
      }
      #setNetworkDialog form details summary {
        margin-bottom: 0.7em;
        cursor: pointer;
        font-size: 0.9em;
      }

      .currentAccessPoint {
        display: none;
      }
      body[data-activeAP] .currentAccessPoint {
        display: block;
      }

      .availableNetworks {
        display: block;
      }
      body[data-savedSSID] .availableNetworks {
        display: none;
      }

      .currentNetwork {
        display: none;
      }
      body[data-savedSSID] .currentNetwork {
        display: block;
      }

      main {
        display: block;
      }
      body[data-loading] main {
        display: none;
      }

      .loading {
        display: none;
      }
      body[data-loading] .loading {
        display: block;
      }
    </style>
  </head>
  <body data-loading>
    <h1>WiFi Settings</h1>
    <div class="loading">Loading...</div>
    <main>
      <section class="currentNetwork">
        <div class="currentNetworkContent content">
          <h2>Currently saved network</h2>
          <p>
            This is the currently saved network. Forget this network if you want
            to connect to another network.
          </p>
          <div class="currentNetworkCard card">
            <div>
              <span id="currentNetworkSSID"></span>
              <span id="currentNetworkStrength" class="iconWrapper">
                <span id="currentNetworkStrengthIcon">
                  <img src="" alt="" />
                </span>
                <span id="currentNetworkStrengthText" class="sr-only"></span>
              </span>
            </div>
            <button id="currentNetworkForget">Forget network</button>
          </div>
        </div>
        <details>
          <summary>Advanced Details</summary>
          <div class="currentNetworkDetails details">
            <dl>
              <div>
                <dt>Hostname</dt>
                <dd>
                  <a id="currentNetworkLink" href=""
                    ><span id="currentNetworkHostname"></span>.local</a
                  >
                </dd>
              </div>
              <div>
                <dt>IP Address</dt>
                <dd><span id="currentNetworkIP"></span></dd>
              </div>
              <div>
                <dt>Gateway</dt>
                <dd><span id="currentNetworkGateway"></span></dd>
              </div>
              <div>
                <dt>Subnet</dt>
                <dd><span id="currentNetworkSubnet"></span></dd>
              </div>
            </dl>
          </div>
        </details>
      </section>

      <section class="availableNetworks">
        <div class="availableNetworksContent content">
          <h2>Connect to a new network</h2>
          <div class="availableNetworksInner">
            <div class="formField">
              <label for="availableNetworksSelect" class="sr-only"
                >Connect to a new network:</label
              >
              <select id="availableNetworksSelect">
                <button>
                  <selectedcontent></selectedcontent>
                </button>
                <option id="availableNetworksNullOption" value="">
                  Select a network
                </option>
                <div id="availableNetworksOptions"></div>
              </select>
            </div>
            <div class="availableNetworksManuallyInsertWrapper">
              or
              <button id="availableNetworksManuallyInsert" class="textButton">
                insert network manually
              </button>
            </div>
          </div>
        </div>
      </section>

      <section class="currentAccessPoint">
        <div class="currentAccessPointContent content">
          <h2>Active access point</h2>
          <p>
            The access point is currently active, because no connection is
            available. By connecting to this WiFi network, all requests are
            redirected to this settings page.
          </p>
          <div class="currentAccessPointCard card">
            <div>
              <dl>
                <div>
                  <dt>Access point name</dt>
                  <dd><span id="currentAPSSID"></span></dd>
                </div>
              </dl>
            </div>
          </div>
        </div>
        <details>
          <summary>Advanced details</summary>
          <div class="currentAPDetails details">
            <dl>
              <div>
                <dt>IP Address</dt>
                <dd><span id="currentAPIP"></span></dd>
              </div>
              <div>
                <dt>Subnet</dt>
                <dd><span id="currentAPSubnet"></span></dd>
              </div>
            </dl>
          </div>
        </details>
      </section>

      <section class="setNetwork">
        <dialog id="setNetworkDialog" aria-labelledby="setNetworkDialogTitle">
          <h2 id="setNetworkDialogTitle">Connect to network</h2>
          <form id="setNetworkForm">
            <div class="formField">
              <label for="setNetworkFormSSID">SSID (Network name)</label>
              <input type="text" name="ssid" id="setNetworkFormSSID" />
            </div>
            <div class="formField">
              <label for="setNetworkFormPassphrase"
                >Passphrase (Network password)</label
              >
              <input
                type="text"
                name="passphrase"
                id="setNetworkFormPassphrase"
              />
            </div>
            <details class="setNetworkFormAdvancedSettings">
              <summary>Advanced settings</summary>
              <div class="formField">
                <label for="setNetworkFormIP">IP Address (manual)</label>
                <input type="text" name="ip" id="setNetworkFormIP" />
              </div>
              <div class="formField">
                <label for="setNetworkFormGateway"
                  >Gateway Address (manual)</label
                >
                <input type="text" name="gateway" id="setNetworkFormGateway" />
              </div>
              <div class="formField">
                <label for="setNetworkFormSubnet">Subnet (manual)</label>
                <input type="text" name="subnet" id="setNetworkFormSubnet" />
              </div>
            </details>
            <button id="setNetworkDialogSubmit" type="submit">Connect</button>
            <button id="setNetworkDialogClose" class="secondaryButton">
              Cancel
            </button>
          </form>
        </dialog>
      </section>
    </main>
    <script>
const icons = {
  "wifi-2": "data:image/svg+xml;base64,PHN2ZyB4bWxucz0iaHR0cDovL3d3dy53My5vcmcvMjAwMC9zdmciIGlkPSJMaXZlbGxvXzEiIHdpZHRoPSIyNi42NyIgaGVpZ2h0PSIyNi42NyIgZGF0YS1uYW1lPSJMaXZlbGxvIDEiIHZpZXdCb3g9IjAgMCAyMCAyMCI+CiAgPGRlZnM+CiAgICA8c3R5bGU+CiAgICAgIC5jbHMtMSwuY2xzLTJ7ZmlsbDpub25lO3N0cm9rZTojMDAwO3N0cm9rZS1saW5lY2FwOnJvdW5kO3N0cm9rZS1saW5lam9pbjpyb3VuZDtzdHJva2Utd2lkdGg6Mi41cHh9LmNscy0ye29wYWNpdHk6LjN9CiAgICA8L3N0eWxlPgogIDwvZGVmcz4KICA8cGF0aCBkPSJNMTAgMTYuMjZ2LS4wMSIgY2xhc3M9ImNscy0xIi8+CiAgPHBhdGggZD0iTTEuNjcgNi42N2M1LTMuNzUgMTEuNjYtMy43NSAxNi42NiAwTTQuMTcgMTBjMy4zMy0yLjUgOC4zMy0yLjUgMTEuNjYgMCIgY2xhc3M9ImNscy0yIi8+CiAgPHBhdGggZD0iTTcuMDggMTIuOTJhNS4zIDUuMyAwIDAgMSA1Ljg0IDAiIGNsYXNzPSJjbHMtMSIvPgo8L3N2Zz4K",
  "wifi-3": "data:image/svg+xml;base64,PHN2ZyB4bWxucz0iaHR0cDovL3d3dy53My5vcmcvMjAwMC9zdmciIGlkPSJMaXZlbGxvXzEiIHdpZHRoPSIyNi42NyIgaGVpZ2h0PSIyNi42NyIgZGF0YS1uYW1lPSJMaXZlbGxvIDEiIHZpZXdCb3g9IjAgMCAyMCAyMCI+CiAgPGRlZnM+CiAgICA8c3R5bGU+CiAgICAgIC5jbHMtMXtmaWxsOm5vbmU7c3Ryb2tlOiMwMDA7c3Ryb2tlLWxpbmVjYXA6cm91bmQ7c3Ryb2tlLWxpbmVqb2luOnJvdW5kO3N0cm9rZS13aWR0aDoyLjVweH0KICAgIDwvc3R5bGU+CiAgPC9kZWZzPgogIDxwYXRoIGQ9Ik0xMCAxNi4yNnYtLjAxIiBjbGFzcz0iY2xzLTEiLz4KICA8cGF0aCBkPSJNMS42NyA2LjY3YzUtMy43NSAxMS42Ni0zLjc1IDE2LjY2IDAiIHN0eWxlPSJvcGFjaXR5Oi4zO2ZpbGw6bm9uZTtzdHJva2U6IzAwMDtzdHJva2UtbGluZWNhcDpyb3VuZDtzdHJva2UtbGluZWpvaW46cm91bmQ7c3Ryb2tlLXdpZHRoOjIuNXB4Ii8+CiAgPHBhdGggZD0iTTQuMTcgMTBjMy4zMy0yLjUgOC4zMy0yLjUgMTEuNjYgME03LjA4IDEyLjkyYTUuMyA1LjMgMCAwIDEgNS44NCAwIiBjbGFzcz0iY2xzLTEiLz4KPC9zdmc+Cg==",
  "wifi-1": "data:image/svg+xml;base64,PHN2ZyB4bWxucz0iaHR0cDovL3d3dy53My5vcmcvMjAwMC9zdmciIGlkPSJMaXZlbGxvXzEiIHdpZHRoPSIyNi42NyIgaGVpZ2h0PSIyNi42NyIgZGF0YS1uYW1lPSJMaXZlbGxvIDEiIHZpZXdCb3g9IjAgMCAyMCAyMCI+CiAgPGRlZnM+CiAgICA8c3R5bGU+CiAgICAgIC5jbHMtMntmaWxsOm5vbmU7c3Ryb2tlOiMwMDA7c3Ryb2tlLWxpbmVjYXA6cm91bmQ7c3Ryb2tlLWxpbmVqb2luOnJvdW5kO3N0cm9rZS13aWR0aDoyLjVweDtvcGFjaXR5Oi4zfQogICAgPC9zdHlsZT4KICA8L2RlZnM+CiAgPHBhdGggZD0iTTEwIDE2LjI2di0uMDEiIHN0eWxlPSJmaWxsOm5vbmU7c3Ryb2tlOiMwMDA7c3Ryb2tlLWxpbmVjYXA6cm91bmQ7c3Ryb2tlLWxpbmVqb2luOnJvdW5kO3N0cm9rZS13aWR0aDoyLjVweCIvPgogIDxwYXRoIGQ9Ik0xLjY3IDYuNjdjNS0zLjc1IDExLjY2LTMuNzUgMTYuNjYgME00LjE3IDEwYzMuMzMtMi41IDguMzMtMi41IDExLjY2IDBNNy4wOCAxMi45MmE1LjMgNS4zIDAgMCAxIDUuODQgMCIgY2xhc3M9ImNscy0yIi8+Cjwvc3ZnPgo=",
  "wifi-0": "data:image/svg+xml;base64,PHN2ZyB4bWxucz0iaHR0cDovL3d3dy53My5vcmcvMjAwMC9zdmciIGlkPSJMaXZlbGxvXzEiIHdpZHRoPSIyNi42NyIgaGVpZ2h0PSIyNi42NyIgZGF0YS1uYW1lPSJMaXZlbGxvIDEiIHZpZXdCb3g9IjAgMCAyMCAyMCI+CiAgPGRlZnM+CiAgICA8c3R5bGU+CiAgICAgIC5jbHMtMXtmaWxsOm5vbmU7c3Ryb2tlOiMwMDA7c3Ryb2tlLWxpbmVjYXA6cm91bmQ7c3Ryb2tlLWxpbmVqb2luOnJvdW5kO3N0cm9rZS13aWR0aDoyLjVweH0KICAgIDwvc3R5bGU+CiAgPC9kZWZzPgogIDxwYXRoIGQ9Ik0xMCAxNy42NmMtLjQ4IDAtLjk3LS4yLTEuMy0uNkwuNzggNy43QTEuNzMgMS43MyAwIDAgMSAuNDMgNi40Yy4wNi0uNDUuMy0uODUuNjUtMS4xMkExNC40OCAxNC40OCAwIDAgMSAxMCAyLjA5YzMuMTQgMCA2LjE0IDEuMDcgOC45MiAzLjIuNC4zMy42Mi43NS42NiAxLjIxLjA0LjQ2LS4xLjktLjQgMS4yNGwtOC4wOCA5LjUxYy0uMzIuMjgtLjcuNC0xLjEuNFoiIHN0eWxlPSJvcGFjaXR5Oi4zIi8+CiAgPHBhdGggZD0iTTEyLjcxIDYuMDUgNy4zIDExLjQ3TTEyLjcxIDExLjQ3IDcuMyA2LjA1IiBjbGFzcz0iY2xzLTEiLz4KPC9zdmc+Cg==",
  "wifi-4": "data:image/svg+xml;base64,PHN2ZyB4bWxucz0iaHR0cDovL3d3dy53My5vcmcvMjAwMC9zdmciIGlkPSJMaXZlbGxvXzEiIHdpZHRoPSIyNi42NyIgaGVpZ2h0PSIyNi42NyIgZGF0YS1uYW1lPSJMaXZlbGxvIDEiIHZpZXdCb3g9IjAgMCAyMCAyMCI+CiAgPGRlZnM+CiAgICA8c3R5bGU+CiAgICAgIC5jbHMtMXtmaWxsOm5vbmU7c3Ryb2tlOiMwMDA7c3Ryb2tlLWxpbmVjYXA6cm91bmQ7c3Ryb2tlLWxpbmVqb2luOnJvdW5kO3N0cm9rZS13aWR0aDoyLjVweH0KICAgIDwvc3R5bGU+CiAgPC9kZWZzPgogIDxwYXRoIGQ9Ik0xMCAxNi4yNnYtLjAxTTEuNjcgNi42N2M1LTMuNzUgMTEuNjYtMy43NSAxNi42NiAwTTQuMTcgMTBjMy4zMy0yLjUgOC4zMy0yLjUgMTEuNjYgME03LjA4IDEyLjkyYTUuMyA1LjMgMCAwIDEgNS44NCAwIiBjbGFzcz0iY2xzLTEiLz4KPC9zdmc+Cg==",
  "wifi-unsecure": "data:image/svg+xml;base64,PHN2ZyB4bWxucz0iaHR0cDovL3d3dy53My5vcmcvMjAwMC9zdmciIGlkPSJMaXZlbGxvXzEiIHdpZHRoPSIyNi42NyIgaGVpZ2h0PSIyNi42NyIgZGF0YS1uYW1lPSJMaXZlbGxvIDEiIHZpZXdCb3g9IjAgMCAyMCAyMCI+CiAgPGRlZnM+CiAgICA8c3R5bGU+CiAgICAgIC5jbHMtMXtmaWxsOm5vbmU7c3Ryb2tlOiMwMDA7c3Ryb2tlLWxpbmVjYXA6cm91bmQ7c3Ryb2tlLWxpbmVqb2luOnJvdW5kO3N0cm9rZS13aWR0aDoyLjVweH0KICAgIDwvc3R5bGU+CiAgPC9kZWZzPgogIDxwYXRoIGQ9Ik05LjU4IDEwSDUuNWEuNS41IDAgMCAwLS41LjV2NS42N2MwIC4yNy4yMi41LjUuNWg5YS41LjUgMCAwIDAgLjUtLjV2LS43NU0xMy4zMyAxMFY2LjY3YzAtMS4xMS0uNjYtMy4zNC0zLjMzLTMuMzRhMy42IDMuNiAwIDAgMC0xLjU2LjMyTTEzLjMzIDEwaDEuMTdjLjI4IDAgLjUuMjIuNS41di4zM002LjY3IDYuNjdWMTBNMi41IDIuNWwxNSAxNSIgY2xhc3M9ImNscy0xIi8+Cjwvc3ZnPgo=",
  "wifi-secure": "data:image/svg+xml;base64,PHN2ZyB4bWxucz0iaHR0cDovL3d3dy53My5vcmcvMjAwMC9zdmciIHdpZHRoPSIyNi42NyIgaGVpZ2h0PSIyNi42NyIgZGF0YS1uYW1lPSJMaXZlbGxvIDEiIHZpZXdCb3g9IjAgMCAyMCAyMCI+CiAgPHBhdGggZD0iTTEzLjMzIDEwaDEuMTdjLjI4IDAgLjUuMjIuNS41djUuNjdhLjUuNSAwIDAgMS0uNS41aC05YS41LjUgMCAwIDEtLjUtLjVWMTAuNWMwLS4yOC4yMi0uNS41LS41aDEuMTdtNi42NiAwVjYuNjdjMC0xLjExLS42Ni0zLjM0LTMuMzMtMy4zNFM2LjY3IDUuNTYgNi42NyA2LjY3VjEwbTYuNjYgMEg2LjY3IiBzdHlsZT0iZmlsbDpub25lO3N0cm9rZTojMDAwO3N0cm9rZS1saW5lY2FwOnJvdW5kO3N0cm9rZS1saW5lam9pbjpyb3VuZDtzdHJva2Utd2lkdGg6Mi41cHgiLz4KPC9zdmc+Cg==",
};
</script>
    <script>
      let pauseLoop = false;

      const fetchStatus = async () => {
        try {
          const response = await fetch("/wifi/api/status");
          if (!response.ok) throw new Error("Response not HTTP OK");
          return await response.json();
        } catch (err) {
          console.error(err);
          throw new Error("Failed to fetch status");
        }
      };

      const fetchScan = async () => {
        try {
          const response = await fetch("/wifi/api/scan");
          if (!response.ok) throw new Error("Response not HTTP OK");
          const data = await response.json();

          data.networks.sort((a, b) => {
            if (b.strength !== a.strength) return b.strength - a.strength;
            return a.ssid.localeCompare(b.ssid);
          });

          return data;
        } catch (err) {
          console.error(err);
          throw new Error("Failed to scan networks");
        }
      };

      const submitNetwork = async ({
        ssid,
        passphrase,
        ip,
        gateway,
        subnet,
      }) => {
        try {
          const payload = {
            ssid,
            passphrase,
            ip,
            gateway,
            subnet,
          };

          const response = await fetch("/wifi/api/connect", {
            method: "POST",
            headers: {
              "Content-Type": "application/json",
            },
            body: JSON.stringify(payload),
          });

          if (!response.ok) {
            const error = await response.text();
            console.error("submitNetwork failed:", error);
          }
        } catch (err) {
          console.error("submitNetwork error:", err);
        }
      };

      const submitForget = async () => {
        try {
          const response = await fetch("/wifi/api/forget", {
            method: "POST",
          });

          if (!response.ok) {
            const error = await response.text();
            console.error("submitForget failed:", error);
          }
        } catch (err) {
          console.error("submitForget error:", err);
        }
      };

      const getStrengthString = (strength) => {
        let strengthString = "Connecting...";
        switch (strength) {
          case 1:
            strengthString = " | Strength level 1 out of 4: weak";
            break;
          case 2:
            strengthString = " | Strength level 2 out of 4: weak";
            break;
          case 3:
            strengthString = " | Strength level 3 out of 4: strong";
            break;
          case 4:
            strengthString = " | Strength level 4 out of 4: excellent";
            break;

          default:
            break;
        }

        return strengthString;
      };

      const loadStatus = async () => {
        const status = await fetchStatus();

        if (status.network.ssid) {
          document.querySelector("#currentNetworkSSID").innerHTML =
            status.network.ssid;

          document
            .querySelector("#currentNetworkStrengthIcon img")
            .setAttribute("src", icons[`wifi-${status.network.strength}`]);
          document.querySelector("#currentNetworkStrengthText").innerHTML =
            getStrengthString(status.network.strength);

          document.querySelector("#currentNetworkHostname").innerHTML =
            status.network.hostname;
          document
            .querySelector("#currentNetworkLink")
            .setAttribute(
              "href",
              "http://" + status.network.hostname + ".local/"
            );
          document.querySelector("#currentNetworkIP").innerHTML =
            status.network.ip;
          document.querySelector("#currentNetworkGateway").innerHTML =
            status.network.gateway;
          document.querySelector("#currentNetworkSubnet").innerHTML =
            status.network.subnet;

          document.body.setAttribute("data-savedSSID", "");
          document.body.removeAttribute("data-loading");
        } else {
          document.body.removeAttribute("data-savedSSID");
        }

        if (status.ap.active) {
          document.querySelector("#currentAPSSID").innerHTML = status.ap.ssid;
          document.querySelector("#currentAPIP").innerHTML = status.ap.ip;
          document.querySelector("#currentAPSubnet").innerHTML =
            status.ap.subnet;
          document.body.setAttribute("data-activeAP", "");
        } else {
          document.body.removeAttribute("data-activeAP");
        }

        return status;
      };

      const loadScan = async (status) => {
        if (!status.network.ssid) {
          const scan = await fetchScan();
          const optionsList = document.querySelector(
            "#availableNetworksOptions"
          );

          if (scan.networks.length > 0) {
            if (optionsList) {
              optionsList.innerHTML = "";
              document.querySelector(
                "#availableNetworksSelect selectedcontent"
              ).innerHTML = "Select a network";
              document.querySelector("#availableNetworksNullOption").innerHTML =
                "Select a network";
            } else {
              const select = document.querySelector("#availableNetworksSelect");
              select.innerHTML = "";
              const option = document.createElement("option");
              option.innerHTML = "Select a network";
              select.append(option);
            }
          } else {
            if (optionsList) {
              optionsList.innerHTML = "";
              document.querySelector(
                "#availableNetworksSelect selectedcontent"
              ).innerHTML = "Scanning networks...";
              document.querySelector("#availableNetworksNullOption").innerHTML =
                "Scanning networks...";
            } else {
              const select = document.querySelector("#availableNetworksSelect");
              select.innerHTML = "";
              const option = document.createElement("option");
              option.innerHTML = "Scanning networks...";
              select.append(option);
            }
          }

          scan.networks.forEach((network) => {
            const option = document.createElement("option");
            option.classList.add("availableNetworksOption");
            option.setAttribute("value", network.ssid);
            option.setAttribute("data-secure", network.secure);

            if (optionsList) {
              const ssidSpan = document.createElement("span");
              ssidSpan.classList.add("networkSSID");
              ssidSpan.innerHTML = network.ssid;
              option.append(ssidSpan);

              const strengthSpan = document.createElement("span");
              strengthSpan.classList.add("networkStrength");
              strengthSpan.classList.add("iconWrapper");

              const strengthIconSpan = document.createElement("span");
              strengthIconSpan.classList.add("networkStrengthIcon");
              const strengthIconSpanImage = document.createElement("img");
              strengthIconSpanImage.setAttribute("alt", "");
              strengthIconSpanImage.setAttribute(
                "src",
                icons[`wifi-${network.strength}`]
              );
              strengthIconSpan.append(strengthIconSpanImage);
              strengthSpan.append(strengthIconSpan);

              const strengthTextSpan = document.createElement("span");
              strengthTextSpan.classList.add("networkStrengthText");
              strengthTextSpan.classList.add("sr-only");
              strengthTextSpan.innerHTML = getStrengthString(network.strength);
              strengthSpan.append(strengthTextSpan);

              option.append(strengthSpan);

              const secureSpan = document.createElement("span");
              secureSpan.classList.add("networkSecure");
              secureSpan.classList.add("iconWrapper");

              const secureIconSpan = document.createElement("span");
              secureIconSpan.classList.add("networkSecureIcon");
              const secureIconSpanImage = document.createElement("img");
              secureIconSpanImage.setAttribute("alt", "");
              secureIconSpanImage.setAttribute(
                "src",
                icons[`wifi-${network.secure ? "" : "un"}secure`]
              );
              secureIconSpan.append(secureIconSpanImage);
              secureSpan.append(secureIconSpan);

              const secureTextSpan = document.createElement("span");
              secureTextSpan.classList.add("networkSecureText");
              secureTextSpan.classList.add("sr-only");
              secureTextSpan.innerHTML = network.secure
                ? " | Secure network"
                : " | Open network";
              secureSpan.append(secureTextSpan);

              option.append(secureSpan);
              optionsList.append(option);
            } else {
              option.innerHTML = `${network.ssid}${
                network.secure ? " | 🔒" : ""
              }`;
              const select = document.querySelector("#availableNetworksSelect");
              select.append(option);
            }
          });

          document.body.removeAttribute("data-loading");
        }
      };

      const openNetworkModal = ({ ssid, secure }) => {
        if (ssid) {
          document.querySelector("#setNetworkFormSSID").value = ssid;
          document
            .querySelector("#setNetworkFormSSID")
            .setAttribute("readOnly", "");
        } else {
          document.querySelector("#setNetworkFormSSID").value = "";
          document
            .querySelector("#setNetworkFormSSID")
            .removeAttribute("readOnly");
        }

        if (!secure) {
          document.querySelector(
            "#setNetworkFormPassphrase"
          ).parentElement.style.display = "none";
        } else {
          document.querySelector(
            "#setNetworkFormPassphrase"
          ).parentElement.style.display = "block";
        }

        document.querySelector("#setNetworkDialog").showModal();
      };

      document
        .querySelector("#availableNetworksSelect")
        .addEventListener("change", (event) => {
          pauseLoop = true;
          const activeOption = event.target.options[event.target.selectedIndex];
          const ssid = event.target.value;
          const secure = activeOption.getAttribute("data-secure") === "true";
          openNetworkModal({ ssid, secure });
        });

      document
        .querySelector("#availableNetworksManuallyInsert")
        .addEventListener("click", () => openNetworkModal({ secure: true }));

      document
        .querySelector("#setNetworkDialogClose")
        .addEventListener("click", (event) => {
          event.preventDefault();
          document.querySelector("#setNetworkDialog").close();
          pauseLoop = false;
          loop();
        });

      document
        .querySelector("#setNetworkForm")
        .addEventListener("submit", async (event) => {
          event.preventDefault();
          const data = new FormData(event.target);
          const ssid = data.get("ssid");
          const passphrase = data.get("passphrase");
          const ip = data.get("ip");
          const gateway = data.get("gateway");
          const subnet = data.get("subnet");

          const submitBtn = document.querySelector("#setNetworkDialogSubmit");
          submitBtn.disabled = true;

          await submitNetwork({
            ssid,
            passphrase,
            ip,
            gateway,
            subnet,
          });

          submitBtn.disabled = false;

          document.querySelector("#setNetworkDialog").close();

          const optionsList = document.querySelector(
            "#availableNetworksOptions"
          );
          if (optionsList) {
            optionsList.innerHTML = "";
          } else {
            const select = document.querySelector("#availableNetworksSelect");
            select.innerHTML = "";
          }

          document.body.setAttribute("data-loading", "");
          setTimeout(async () => {
            await loop();
            pauseLoop = false;
          }, 500);
        });

      document
        .querySelector("#currentNetworkForget")
        .addEventListener("click", async () => {
          await submitForget();
          document.body.setAttribute("data-loading", "");
          setTimeout(async () => {
            await loop();
            pauseLoop = false;
          }, 500);
        });

      const loop = async () => {
        const status = await loadStatus();
        await loadScan(status);
      };

      loop();

      setInterval(() => {
        if (!pauseLoop) loop();
      }, 5000);
    </script>
  </body>
</html>

)rawliteral";

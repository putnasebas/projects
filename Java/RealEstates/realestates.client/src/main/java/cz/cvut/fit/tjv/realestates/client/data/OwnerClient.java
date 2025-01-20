package cz.cvut.fit.tjv.realestates.client.data;

import cz.cvut.fit.tjv.realestates.client.model.OwnerDTO;
import cz.cvut.fit.tjv.realestates.client.model.PropertyDTO;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.http.MediaType;
import org.springframework.stereotype.Component;
import org.springframework.web.reactive.function.client.WebClient;
import reactor.core.publisher.Flux;
import reactor.core.publisher.Mono;

@Component
public class OwnerClient {
    private final static String OWNER_URI = "/{id}";
    private final WebClient webClient;

    public OwnerClient(@Value("${backend.url}") String backendUrl) {
        this.webClient = WebClient.create(backendUrl + "/rest/api/owner");
    }

    public Mono<OwnerDTO> getOwnerById(String id) {
        return webClient.get()
                .uri(OWNER_URI, id)
                .retrieve()
                .bodyToMono(OwnerDTO.class);
    }

    public Flux<OwnerDTO> getAllOwners() {
        return webClient.get()
                .retrieve()
                .bodyToFlux(OwnerDTO.class);
    }

    public Mono<OwnerDTO> createOwner(OwnerDTO ownerDTO) {
        return webClient.post()
                .contentType(MediaType.APPLICATION_JSON)
                .bodyValue(ownerDTO)
                .retrieve()
                .bodyToMono(OwnerDTO.class);
    }

    public Mono<OwnerDTO> updateOwner(OwnerDTO ownerDTO, String email) {
        return webClient.put()
                .uri(OWNER_URI, email)
                .contentType(MediaType.APPLICATION_JSON)
                .bodyValue(ownerDTO)
                .retrieve()
                .bodyToMono(OwnerDTO.class);
    }

    public Mono<Void> deleteOwnerById(String id) {
        return webClient.delete()
                .uri(OWNER_URI, id)
                .retrieve()
                .bodyToMono(Void.class);
    }

    public Flux<PropertyDTO> addOwnerToProperty(String email, PropertyDTO propertyDTO) {
        return webClient.put()
                .uri(OWNER_URI + "/properties", email)
                .contentType(MediaType.APPLICATION_JSON)
                .bodyValue(propertyDTO)
                .retrieve()
                .bodyToFlux(PropertyDTO.class);
    }

    public Mono<Void> removeOwnerFromProperty(String email, Long Id) {
        return webClient.delete()
                .uri(OWNER_URI + "/properties/" + Id, email)
                .retrieve()
                .bodyToMono(Void.class);
    }

}
